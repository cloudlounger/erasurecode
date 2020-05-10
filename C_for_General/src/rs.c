/*******************************************************************************************
 *  @file      rs.c 2017\4\6 12:10:32 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "rs.h"
#include "auxiliary.h"
#include "x86cpu.h"
#include "rsencode.h"
#include "rsdecode.h"
#include <string.h>
// ------------------------------------------------------------------------------------------
// 
int initializeRS(size_t N, size_t K)
{
    if ((N == 0)||(K == 0))
    {
        return 1;
    }

    if (N <= K)
    {
        return 2;
    }

    if ((N + K) > 255)
    {
        return 3;
    }

    if (K > 16)
    {
        return 4;
    }


    g_RS_N   = N;
    g_RS_K   = K;
    g_RS_ALL = g_RS_K + g_RS_N;

    return 0;
}


// ------------------------------------------------------------------------------------------
// 
size_t getDataN()
{
    return g_RS_N;
}

// ------------------------------------------------------------------------------------------
// 
size_t getCheckK()
{
    return g_RS_K;
}

// ------------------------------------------------------------------------------------------
// 
const char* errorToStr(int err)
{
    switch (err)
    {
    case -1:
        return "Memory allocation error";
    case 0:
        return "No error";
    case 1:
        return "N and K can not be zero";
    case 2:
        return "N must be greater than K";
    case 3:
        return "The sum of N and K must be less than 255";
    case 4:
        return "K must be less than or equal to 16";
    default:
        return "Unknown error";
    }
}

// ------------------------------------------------------------------------------------------
// 
int encodeRS(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    int result = 0;

    SIMDInstruction simdinstr = getSIMDInstruction();
    switch (simdinstr)
    {
    case HW_AVX2:
        result = encodeAVX2(dataSeg, checkSeg, count);
        break;
    case HW_AVX:
        result = encodeAVX(dataSeg, checkSeg, count);
        break;
    case HW_SSE42:
        result = encodeSSE42(dataSeg, checkSeg, count);
        break;
    default:
        result = encodeNoSIMD(dataSeg, checkSeg, count);
    }

    return result;
}

// ------------------------------------------------------------------------------------------
// 
ErrorLocationInfo* createErrorLocationInfo(const size_t* locationList, const size_t locationLength)
{
    ErrorLocationInfo* pResult = malloc(sizeof(ErrorLocationInfo));

    pResult->m_nErrorCount = locationLength;
    /*拷贝错误列表*/
    pResult->m_pErrorLocationList = malloc(sizeof(size_t) * (pResult->m_nErrorCount));
    memcpy(pResult->m_pErrorLocationList, locationList, sizeof(size_t) * (pResult->m_nErrorCount));
    /*计算gamma*/
    pResult->m_pGamma = malloc(sizeof(gfExp)*((pResult->m_nErrorCount) + 1));
    memset(pResult->m_pGamma, 0, sizeof(gfExp)*(pResult->m_nErrorCount));
    pResult->m_pGamma[pResult->m_nErrorCount] = 1;
    for (size_t i = 0; i < (pResult->m_nErrorCount); ++i)
    {
        unsigned char mutlpoly = AlphaT[g_RS_ALL - 1 - pResult->m_pErrorLocationList[i]];

        for (size_t j = ((pResult->m_nErrorCount) - 1 - i); j < (pResult->m_nErrorCount); ++j)
        {
            pResult->m_pGamma[j] ^= LowT[pResult->m_pGamma[j + 1]][mutlpoly & 0xf] ^
                HighT[pResult->m_pGamma[j + 1]][(mutlpoly & 0xf0) >> 4];
        }
    }
    // 计算gamma derivative
    pResult->m_pGammaDerivative = malloc(sizeof(gfExp)*(pResult->m_nErrorCount));

    for (size_t i = 0; i < (pResult->m_nErrorCount); ++i)
    {
        size_t sub = (pResult->m_nErrorCount) - 1 - i;
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

// ------------------------------------------------------------------------------------------
// 
int decodeRS(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    int result = 0;

    SIMDInstruction simdinstr = getSIMDInstruction();
    switch (simdinstr)
    {
    case HW_AVX2:
        result = decodeAVX2(pDatas, pErInfo, count);
        break;
    case HW_AVX:
        result = decodeAVX(pDatas, pErInfo, count);
        break;
    case HW_SSE42:
        result = decodeSSE42(pDatas, pErInfo, count);
        break;
    default:
        result = decodeNoSIMD(pDatas, pErInfo, count);
    }

    return result;
}

// ------------------------------------------------------------------------------------------
// 
void terminateRS()
{
    g_RS_N   = 0;
    g_RS_K   = 0;
    g_RS_ALL = 0;
}

// 
// ------------------------------------------------------------------------------------------
