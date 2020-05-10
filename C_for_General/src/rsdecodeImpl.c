/*******************************************************************************************
 *  @file      rsdecodeImpl.c 2017\4\6 15:37:45 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "rsdecode.h"
#include "auxiliary.h"
#include <string.h>
// ------------------------------------------------------------------------------------------
//
static int decodeNoSIMDImpl(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    const size_t  errorCount          = pErInfo->m_nErrorCount;

    gfExp** syndromArray = (gfExp**)malloc(sizeof(gfExp*)*g_RS_K);
    if (NULL == syndromArray)
    {
        return -1;
    }

    syndromArray[0] = (gfExp*)calloc(count * g_RS_K, sizeof(gfExp));
    if (NULL == syndromArray[0])
    {
        free(syndromArray);
        return -1;
    }

    for (size_t i = 0; i < g_RS_K; ++i)
    {
        syndromArray[i] = syndromArray[0] + i * count;
    }

    // 计算 syndromArray  i = 0 特例
    for (size_t j = 0; j < g_RS_ALL; ++j)
    {
        for (size_t col = 0; col < count; ++col)
        {
            syndromArray[g_RS_K - 1][col] ^= pDatas[j][col];
        }
    }

    for (size_t i = 1; i < g_RS_K; ++i)
    {
        for (size_t j = 0; j < g_RS_ALL - 1; ++j)
        {
            const unsigned char expSub =  AlphaT[i* (g_RS_ALL - 1 - j) % 255];
            const gfExp* lowTable      =  LowT[expSub];
            const gfExp* highTable     =  HighT[expSub];

            for (size_t col = 0; col < count; ++col)
            {
                gfExp sub  = pDatas[j][col];
                syndromArray[g_RS_K - 1 - i][col] ^= (lowTable[sub & 0xf] ^ highTable[(sub & 0xf0) >> 4]);
            }
        }

        for (size_t col = 0; col < count; ++col)
        {
            syndromArray[g_RS_K - 1 - i][col] ^= pDatas[g_RS_ALL - 1][col];
        }
    }

    size_t  omeagLen = g_RS_K + errorCount;

    gfExp** omegaArry = (gfExp**)malloc(sizeof(gfExp*) * omeagLen);
    if (NULL == omegaArry)
    {
        free(syndromArray[0]);
        free(syndromArray);
        return -1;
    }

    omegaArry[0] = (gfExp*)calloc(count * omeagLen, sizeof(gfExp));

    if (NULL == omegaArry[0])
    {
        free(syndromArray[0]);
        free(syndromArray);
        free(omegaArry);
        return -1;
    }

    for (size_t i = 0; i < omeagLen; ++i)
    {
        omegaArry[i] = omegaArry[0] + i *count;
    }

    gfExp *ppResult = omegaArry[omeagLen - 1];
    gfExp *ppCurrentResult = NULL;

    for (size_t i = errorCount + 1; i > 0; --i)
    {
        ppCurrentResult = ppResult;
        ppResult -= count;


        const gfExp* lowTable      =   LowT[pErInfo->m_pGamma[i - 1]];
        const gfExp* highTable     =  HighT[pErInfo->m_pGamma[i - 1]];

        for (size_t j = g_RS_K; j > 0; --j)
        {
            const gfExp* ptemSyndromArray  = syndromArray[j - 1];
            for (size_t col = 0; col < count; ++col)
            {
                const gfExp sub = ptemSyndromArray[col];
                ppCurrentResult[col] ^= (lowTable[sub & 0xf] ^ highTable[(sub & 0xf0) >> 4]);
            }
            ppCurrentResult -= count;
        }
    }

    free(syndromArray[0]);
    free(syndromArray);

    gfExp* omegaValueArray = (gfExp*)malloc(sizeof(gfExp)*count);
    if (NULL == omegaValueArray)
    {
        free(omegaArry[0]);
        free(omegaArry);
        return -1;
    }


    for (size_t errsub = 0; errsub < errorCount; ++errsub)
    {
        const size_t errorSite = pErInfo->m_pErrorLocationList[errsub];

        memset(omegaValueArray, 0, sizeof(gfExp)*count);

        for (size_t i = 0; i < g_RS_K; ++i)
        {
            gfExp temp = AlphaT[((256 - g_RS_ALL + errorSite)* (g_RS_K - 1 - i)) % 255];

            const gfExp* lowTable      =   LowT[temp];
            const gfExp* highTable     =  HighT[temp];


            const gfExp* ptempOmegaArry    = omegaArry[i + errorCount];
            for (size_t col = 0; col < count; ++col)
            {
                const gfExp sub       = ptempOmegaArry[col];
                omegaValueArray[col] ^= (lowTable[sub & 0xf] ^ highTable[(sub & 0xf0) >> 4]);
            }
        }

        gfExp denominator = 0;
        for (size_t i = 0; i < errorCount; ++i)
        {
            unsigned char col = AlphaT[((256 - g_RS_ALL + errorSite) * (errorCount - 1 - i)) % 255];
            denominator ^=  LowT[pErInfo->m_pGammaDerivative[i]][col & 0xf] ^
                HighT[pErInfo->m_pGammaDerivative[i]][(col & 0xf0) >> 4];
        }

        denominator = InvT[denominator];
        const gfExp tempAlpha                = AlphaT[g_RS_ALL - 1 - errorSite];

        const gfExp* omegaValueLowtable   = LowT[tempAlpha];  // 低位表
        const gfExp* omegaValueHightable  = HighT[tempAlpha]; // 高位表

        const gfExp* denominatorLowtable  = LowT[denominator];  // 低位表
        const gfExp* denominatorHightable = HighT[denominator]; // 高位表

        for (size_t col = 0; col < count; ++col)
        {
            const gfExp sub         = omegaValueArray[col];
            const gfExp omegaSub    = (omegaValueLowtable[sub & 0xf] ^ omegaValueHightable[(sub & 0xf0) >> 4]);
            pDatas[errorSite][col] ^= (denominatorLowtable[omegaSub & 0xf] ^ denominatorHightable[(omegaSub & 0xf0) >> 4]);
        }
    }

    free(omegaValueArray);
    free(omegaArry[0]);
    free(omegaArry);

    return 0;
}

// ------------------------------------------------------------------------------------------
//
static int decode1CheckNoSIMDImpl(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    size_t* pSubList = malloc(sizeof(size_t)*g_RS_N);
    size_t* pInterator = pSubList;
    for (size_t i = 0; i < g_RS_ALL; ++i)
    {
        if (i != pErInfo->m_pErrorLocationList[0])
        {
            *pInterator = i;
            ++pInterator;
        }
    }

    gfExp* pdest = pDatas[pErInfo->m_pErrorLocationList[0]];

    memcpy(pdest, pDatas[pSubList[0]], count);

    for (size_t i = 1; i < g_RS_N; ++i)
    {
        gfExp* psrc  = pDatas[pSubList[i]];
        for (size_t col = 0; col < count; ++col)
        {
            pdest[col] ^= psrc[col];
        }
    }

    free(pSubList);

    return 0;
}

// ------------------------------------------------------------------------------------------
// 
int decodeNoSIMD(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    int result = 0;
    if (g_RS_K == 1)
    {
        result = decode1CheckNoSIMDImpl(pDatas, pErInfo, count);
    }
    else
    {
        result = decodeNoSIMDImpl(pDatas, pErInfo, count);
    }

    return result;
}
// 
// ------------------------------------------------------------------------------------------
