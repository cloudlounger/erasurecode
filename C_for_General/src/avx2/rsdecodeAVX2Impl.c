/*******************************************************************************************
 *  @file      rsdecodeAVX2Impl.c 2017\4\24 15:15:03 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "../rsdecode.h"
#include "../auxiliary.h"
#include <string.h>
#include <immintrin.h>
// ------------------------------------------------------------------------------------------
//
static int decodeAVX2Impl(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    static const size_t alignedLength = 32;

    const __m256i maskLow          = _mm256_set1_epi8(0xf); // 低位掩码
    const __m256i maskHigh         = _mm256_set1_epi8(0xf0);// 高位掩码

    const size_t  the32Mode           = count / alignedLength;
    const size_t  errorCount          = pErInfo->m_nErrorCount;


    gfExp** syndromArray = (gfExp**)malloc(sizeof(gfExp*)*g_RS_K);
    if (NULL == syndromArray)
    {
        return -1;
    }

    if (0 != posix_memalign((void**)(&syndromArray[0]), alignedLength, count * g_RS_K * sizeof(gfExp)))
    {
        free(syndromArray);
        return -1;
    }

    memset(syndromArray[0], 0, count * g_RS_K);
    for (int i = 0; i < g_RS_K; ++i)
    {
        syndromArray[i] = syndromArray[0] + i * count;
    }

    // 计算 syndromArray  i = 0 特例
    for (size_t j = 0; j < g_RS_ALL; ++j)
    {
        __m256i* pdest = (__m256i*)syndromArray[g_RS_K - 1];
        __m256i* psrc1 = pdest;
        __m256i* psrc2 = (__m256i*)pDatas[j];

        for (size_t col = 0; col < the32Mode; ++col)
        {
            *pdest++ = _mm256_xor_si256(*psrc1++, *psrc2++);
        }
    }

    for (size_t i = 1; i < g_RS_K; ++i)
    {
        for (size_t j = 0; j < g_RS_ALL - 1; ++j)
        {
            const unsigned char expSub = AlphaT[i* (g_RS_ALL - 1 - j) % 255];
            const __m256i lowtable  = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)LowT[expSub]));  // 低位表
            const __m256i hightable = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)HighT[expSub])); // 高位表

            const gfExp* pDataCol   = pDatas[j];
            __m256i* pdest          = (__m256i*)syndromArray[g_RS_K - 1 - i];
            __m256i* psrc1          = pdest;

            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                const size_t col   = colsub * alignedLength;

                __m256i data       = _mm256_load_si256((const __m256i*)(pDataCol + col));
                __m256i lowData    = _mm256_shuffle_epi8(lowtable, _mm256_and_si256(data, maskLow));
                __m256i highData   = _mm256_srli_epi64(_mm256_and_si256(data, maskHigh), 4);
                highData           = _mm256_shuffle_epi8(hightable, highData);
                data               = _mm256_xor_si256(lowData, highData);

                *pdest++           = _mm256_xor_si256(*psrc1++, data);
            }
        }

        __m256i* pdest = (__m256i*)syndromArray[g_RS_K - 1 - i];
        __m256i* psrc1 = (__m256i*)syndromArray[g_RS_K - 1 - i];
        __m256i* psrc2 = (__m256i*)pDatas[g_RS_ALL - 1];

        for (size_t col = 0; col < the32Mode; ++col)
        {

            *pdest++ = _mm256_xor_si256(*psrc1++, *psrc2++);
        }
    }

    size_t  omeagLen  = g_RS_K + errorCount;
    gfExp** omegaArry = (gfExp**)malloc(sizeof(gfExp*) * omeagLen);
    if (NULL == omegaArry)
    {
        FREE_ALIGNED(syndromArray[0]);
        free(syndromArray);
        return -1;
    }


    if (0 != posix_memalign((void**)(&omegaArry[0]), alignedLength, count * omeagLen * sizeof(gfExp)))
    {
        FREE_ALIGNED(syndromArray[0]);
        free(syndromArray);

        free(omegaArry);
        return -1;
    }

    memset(omegaArry[0], 0, count * omeagLen);
    for (size_t i = 0; i < omeagLen; ++i)
    {
        omegaArry[i] = omegaArry[0] + i * count;
    }

    gfExp* ppResult        = omegaArry[omeagLen - 1];
    gfExp* ppCurrentResult = NULL;

    for (size_t i = errorCount + 1; i > 0; --i)
    {
        ppCurrentResult = ppResult;
        ppResult -= count;

        const __m256i lowtable  = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)LowT[pErInfo->m_pGamma[i - 1]]));  // 低位表
        const __m256i hightable = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)HighT[pErInfo->m_pGamma[i - 1]])); // 高位表

        for (size_t j = g_RS_K; j > 0; --j)
        {
            const gfExp* ptemSyndromArray  = syndromArray[j - 1];
            __m256i*     pdest             = (__m256i*)ppCurrentResult;
            __m256i*     psrc1             = pdest;

            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                const size_t col           = colsub * alignedLength;
                __m256i data               = _mm256_load_si256((const __m256i*)(ptemSyndromArray + col));
                __m256i lowData            = _mm256_shuffle_epi8(lowtable, _mm256_and_si256(data, maskLow));
                __m256i highData           = _mm256_srli_epi64(_mm256_and_si256(data, maskHigh), 4);
                highData                   = _mm256_shuffle_epi8(hightable, highData);
                data                       = _mm256_xor_si256(lowData, highData);
                *pdest++                   = _mm256_xor_si256(*psrc1++, data);
            }
            ppCurrentResult -= count;
        }
    }
    FREE_ALIGNED(syndromArray[0]);
    free(syndromArray);

    gfExp* omegaValueArray = NULL;
    if (0 != posix_memalign((void**)(&omegaValueArray), alignedLength, count * sizeof(gfExp)))
    {
        FREE_ALIGNED(omegaArry[0]);
        free(omegaArry);
        return -1;
    }

    for (size_t errsub = 0; errsub < errorCount; ++errsub)
    {
        const size_t errorSite = pErInfo->m_pErrorLocationList[errsub];

        memset(omegaValueArray, 0, sizeof(gfExp)*count);

        for (size_t i = 0; i < g_RS_K; ++i)
        {
            gfExp temp                     = AlphaT[((256 - g_RS_ALL + errorSite)* (g_RS_K - 1 - i)) % 255];

            const __m256i lowtable  = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)LowT[temp]));  // 低位表
            const __m256i hightable = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)HighT[temp])); // 高位表


            const gfExp* ptempOmegaArry    = omegaArry[i + errorCount];
            __m256i* pdest                 = (__m256i*)omegaValueArray;
            __m256i* psrc1                 = (__m256i*)omegaValueArray;

            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                const size_t col           = colsub * alignedLength;
                __m256i data               = _mm256_load_si256((const __m256i*)(ptempOmegaArry + col));
                __m256i lowData            = _mm256_shuffle_epi8(lowtable, _mm256_and_si256(data, maskLow));
                __m256i highData           = _mm256_srli_epi64(_mm256_and_si256(data, maskHigh), 4);
                highData                   = _mm256_shuffle_epi8(hightable, highData);
                data                       = _mm256_xor_si256(lowData, highData);
                *pdest++                   = _mm256_xor_si256(*psrc1++, data);
            }
        }


        gfExp denominator    = 0;
        for (size_t i = 0; i < errorCount; ++i)
        {

            unsigned char col = AlphaT[((256 - g_RS_ALL + errorSite) * (errorCount - 1 - i)) % 255];

            denominator ^=  LowT[pErInfo->m_pGammaDerivative[i]][col & 0xf] ^
                HighT[pErInfo->m_pGammaDerivative[i]][(col & 0xf0) >> 4];
        }
        denominator = InvT[denominator];

        const gfExp tempAlpha              = AlphaT[g_RS_ALL - 1 - errorSite];

        const __m256i omegaValueLowtable   = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)LowT[tempAlpha]));  // 低位表
        const __m256i omegaValueHightable  = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)HighT[tempAlpha])); // 高位表

        const __m256i denominatorLowtable  = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)LowT[denominator]));  // 低位表
        const __m256i denominatorHightable = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)HighT[denominator])); // 高位表

        __m256i* pdest                     = (__m256i*)pDatas[errorSite];
        __m256i* psrc1                     = (__m256i*)pdest;

        for (size_t colsub = 0; colsub < the32Mode; ++colsub)
        {
            const size_t col = colsub * alignedLength;
            __m256i data     = _mm256_load_si256((const __m256i*)(omegaValueArray + col));

            __m256i lowData  = _mm256_shuffle_epi8(omegaValueLowtable, _mm256_and_si256(data, maskLow));
            __m256i highData = _mm256_srli_epi64(_mm256_and_si256(data, maskHigh), 4);
            highData         = _mm256_shuffle_epi8(omegaValueHightable, highData);

            data             = _mm256_xor_si256(lowData, highData);

            lowData          = _mm256_shuffle_epi8(denominatorLowtable, _mm256_and_si256(data, maskLow));
            highData         = _mm256_srli_epi64(_mm256_and_si256(data, maskHigh), 4);
            highData         = _mm256_shuffle_epi8(denominatorHightable, highData);

            data             = _mm256_xor_si256(lowData, highData);
            *pdest++         = _mm256_xor_si256(*psrc1++, data);
        }
    }

    FREE_ALIGNED(omegaValueArray);
    FREE_ALIGNED(omegaArry[0]);
    free(omegaArry);

    return 0;
}
// ------------------------------------------------------------------------------------------
//
static int decode1CheckAVX2Impl(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    static const size_t alignedLength = 32;
    const        size_t the32Mode     = count / alignedLength;

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

    memcpy(pDatas[pErInfo->m_pErrorLocationList[0]], pDatas[pSubList[0]], count);

    for (size_t i = 1; i < g_RS_N; ++i)
    {
        __m256i* psrc1 = (__m256i*)pDatas[pSubList[i]];
        __m256i* psrc2 = (__m256i*)pDatas[pErInfo->m_pErrorLocationList[0]];
        __m256i* pdest = (__m256i*)pDatas[pErInfo->m_pErrorLocationList[0]];

        for (size_t col = 0; col < the32Mode; ++col)
        {
            *pdest++ = _mm256_xor_si256(*psrc1++, *psrc2++);
        }
    }

    free(pSubList);

    return 0;
}

// ------------------------------------------------------------------------------------------
// 
int decodeAVX2(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    int result = 0;
    if (g_RS_K == 1)
    {
        result =  decode1CheckAVX2Impl(pDatas, pErInfo, count);
    }
    else
    {
        result = decodeAVX2Impl(pDatas, pErInfo, count);
    }

    return result;
}
// 
// ------------------------------------------------------------------------------------------
