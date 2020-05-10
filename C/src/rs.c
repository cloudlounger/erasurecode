/*******************************************************************************
 *               rs.c implements the the reedsolomon 编码与解码过程
 * @file         rs.c 2013\08\30 17:40:04 $
 * @author       Wang Xiaotao<wangxiaotao1980@gmail.com>
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rs.h"
#include "RSGlobal.h"
#include "GaliosField8GeneralPoly_0.h"
#include "GaliosField8Table_0.h"

/*******************************************************************************/

#define  AlphaT   g_galoisField8AlphaTable
#define  MulT     g_galoisField8MultiplyTable
#define  DivT     g_galoisField8DivideTable
#define  ExpT     g_galoisField8ExponentTable

int initializeRS(int N, int K)
{

    g_RS_N = N;
    g_RS_K = K;
    g_RS_ALL = N + K;
    g_RS_pGenerator = pGenerator[K];

    return 0;
}

void encodeRS(gfExp** dataSeg, gfExp** checkSeg, const int count)
{
    for(int col = 0; col < count; ++col)
    {
        for (int row = 0; row < g_RS_K; ++row)
        {
            checkSeg[row][col] = dataSeg[row][col];
        }
    //}

    gfExp quotient = 0;

    //for(int col = 0; col < count; ++col)
    //{

        for (int row = g_RS_K; row < g_RS_ALL; ++row)
        {
            quotient = DivT[checkSeg[0][col]][g_RS_pGenerator[0]];

            for (int j = 0; j < (g_RS_K - 1); ++j)
            {
                checkSeg[j][col] = checkSeg[j + 1][col] ^ MulT[quotient][g_RS_pGenerator[j + 1]];
            }

            gfExp tempUC = MulT[quotient][g_RS_pGenerator[g_RS_K]];
            checkSeg[g_RS_K - 1][col] = (row < g_RS_N) ? (dataSeg[row][col] ^ tempUC) : ((gfExp) (0) ^ tempUC);
        }
    }
}

ErrorLocationInfo* createErrorLocationInfo( const int* locationList
                                                       , const int locationLength)
{
    ErrorLocationInfo* pResult = (ErrorLocationInfo*)malloc(sizeof(ErrorLocationInfo));

    pResult->m_nErrorCount = locationLength;
    //拷贝错误列表
    pResult->m_pErrorLocationList = (int*)malloc(sizeof(int) * (pResult->m_nErrorCount));
    memcpy(pResult->m_pErrorLocationList, locationList, sizeof(int) * (pResult->m_nErrorCount));
    //计算gamma
    pResult->m_pGamma = (gfExp*)malloc(sizeof(gfExp)*((pResult->m_nErrorCount) + 1));
    memset(pResult->m_pGamma, 0, sizeof(gfExp)*(pResult->m_nErrorCount));
    pResult->m_pGamma[pResult->m_nErrorCount] = 1;
    for (int i = 0; i < (pResult->m_nErrorCount); ++i)
    {
        unsigned char mutlpoly = AlphaT[g_RS_ALL - 1 - pResult->m_pErrorLocationList[i]];

        for (int j = ((pResult->m_nErrorCount) - 1 - i); j < (pResult->m_nErrorCount); ++j)
        {
            pResult->m_pGamma[j] ^= MulT[pResult->m_pGamma[j + 1]][mutlpoly];
        }
    }
    // 计算gamma derivative
    pResult->m_pGammaDerivative = (gfExp*)malloc(sizeof(gfExp)*(pResult->m_nErrorCount));

    for (int i = 0; i < (pResult->m_nErrorCount); ++i)
    {
        int sub = (pResult->m_nErrorCount) - 1 - i;
        pResult->m_pGammaDerivative[sub] = (1 & i) ? 0 : pResult->m_pGamma[sub];
    }

    return pResult;
}

void deleteErrorLocationInfo(ErrorLocationInfo* errorInfo)
{
    free(errorInfo->m_pGammaDerivative);
    free(errorInfo->m_pGamma);
    free(errorInfo->m_pErrorLocationList);
    free(errorInfo);
}

void decodeRS(gfExp** pDatas, const ErrorLocationInfo* pErInfo, const int count)
{

    int omeagLen = g_RS_K + (pErInfo->m_nErrorCount);
    gfExp syndrom[g_RS_K];
    gfExp omega[omeagLen];

    for(int col = 0; col < count; ++col)
    {
        // computer sydrom
        gfExp returnValue = 0;
        for (int i = 0; i < g_RS_K; ++i)
        {
            syndrom[g_RS_K - 1 - i] = 0;
            for (int j = 0; j < g_RS_ALL; ++j)
            {
                syndrom[g_RS_K - 1 - i] ^= MulT[ExpT[AlphaT[i]][g_RS_ALL - 1 -j]][pDatas[j][col]] ;
            }

            returnValue |= syndrom[g_RS_K - 1 - i];
        }

        if (!returnValue) // returnValue为0 没有错误值，直接越过（当错误过多也可能产生判断错误）
        {
            continue;
        }

//        for (int i = 0; i < (g_RS_K + (pErInfo->m_nErrorCount)); ++i)
//        {
//            omega[i] = 0;
//        }

        memset(omega, 0, sizeof(gfExp)*omeagLen);

        gfExp* pResult = omega + omeagLen -1;
        gfExp* pCurrentResult = 0;
        for (int i = (pErInfo->m_nErrorCount) + 1; i > 0; --i)
        {
            pCurrentResult = pResult;

            for (int j = g_RS_K; j > 0; --j)
            {
                (*pCurrentResult) ^= MulT[pErInfo->m_pGamma[i - 1]][syndrom[j - 1]];
                --pCurrentResult;
            }
            --pResult;
        }

        for (int errsub = 0; errsub < (pErInfo->m_nErrorCount); ++errsub)
        {
            int alpha_inverse = AlphaT[255 -(g_RS_ALL - 1 - (pErInfo->m_pErrorLocationList[errsub]))];

            gfExp omegaValue  = 0;
            for (int i = 0; i < g_RS_K; ++i)
            {
                omegaValue ^= MulT[omega[i + pErInfo->m_nErrorCount]][ExpT[alpha_inverse][g_RS_K - 1 - i]];
            }

            gfExp numerator = MulT[omegaValue][AlphaT[g_RS_ALL - 1 - (pErInfo->m_pErrorLocationList[errsub])]];
            gfExp denominator = 0;
            for (int i = 0; (i < pErInfo->m_nErrorCount); ++i)
            {
                denominator ^= MulT[pErInfo->m_pGammaDerivative[i]][ExpT[alpha_inverse][(pErInfo->m_nErrorCount) - 1 - i]];
            }

            pDatas[pErInfo->m_pErrorLocationList[errsub]][col] ^= DivT[numerator][denominator];
        }
    }
}

/*******************************************************************************/
