/*******************************************************************************************
 *  @file      rsdecodeSSE42Impl.c 2017\6\12 12:43:16 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "../rsdecode.h"
#include "../auxiliary.h"
#include <string.h>
#include <nmmintrin.h>
// ------------------------------------------------------------------------------------------
//
static int decodeSSE42Impl(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    static const size_t alignedLength = 16;

    const __m128i maskLow    = _mm_set1_epi8(0xf); // 低位掩码
    const __m128i maskHigh   = _mm_set1_epi8(0xf0);// 高位掩码

    const size_t  the16Mode  = count / alignedLength;
    const size_t  errorCount = pErInfo->m_nErrorCount;


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
        __m128i* pdest = (__m128i*)syndromArray[g_RS_K - 1];
        __m128i* psrc1 = pdest;
        __m128i* psrc2 = (__m128i*)pDatas[j];

        for (size_t col = 0; col < the16Mode; ++col)
        {
            *pdest++ = _mm_xor_si128(*psrc1++, *psrc2++);
        }
    }

    for (size_t i = 1; i < g_RS_K; ++i)
    {
        for (size_t j = 0; j < g_RS_ALL - 1; ++j)
        {
            const unsigned char expSub = AlphaT[i* (g_RS_ALL - 1 - j) % 255];
            const __m128i lowtable     = _mm_load_si128((const __m128i*)LowT[expSub]);  // 低位表
            const __m128i hightable    = _mm_load_si128((const __m128i*)HighT[expSub]); // 高位表

            const gfExp* pDataCol      = pDatas[j];
            __m128i* pdest             = (__m128i*)syndromArray[g_RS_K - 1 - i];
            __m128i* psrc1             = pdest;

            for (size_t colsub         = 0; colsub < the16Mode; ++colsub)
            {
                const size_t col       = colsub * alignedLength;

                __m128i data           = _mm_load_si128((const __m128i*)(pDataCol + col));
                __m128i lowData        = _mm_shuffle_epi8(lowtable, _mm_and_si128(data, maskLow));
                __m128i highData       = _mm_srli_epi64(_mm_and_si128(data, maskHigh), 4);
                highData               = _mm_shuffle_epi8(hightable, highData);
                data                   = _mm_xor_si128(lowData, highData);

                *pdest++               = _mm_xor_si128(*psrc1++, data);
            }
        }

        __m128i* pdest = (__m128i*)syndromArray[g_RS_K - 1 - i];
        __m128i* psrc1 = (__m128i*)syndromArray[g_RS_K - 1 - i];
        __m128i* psrc2 = (__m128i*)pDatas[g_RS_ALL - 1];

        for (size_t col = 0; col < the16Mode; ++col)
        {

            *pdest++ = _mm_xor_si128(*psrc1++, *psrc2++);
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

        const __m128i lowtable  = _mm_load_si128((const __m128i*)LowT[pErInfo->m_pGamma[i - 1]]);  // 低位表
        const __m128i hightable = _mm_load_si128((const __m128i*)HighT[pErInfo->m_pGamma[i - 1]]); // 高位表

        for (size_t j = g_RS_K; j > 0; --j)
        {
            const gfExp* ptemSyndromArray  = syndromArray[j - 1];
            __m128i*     pdest             = (__m128i*)ppCurrentResult;
            __m128i*     psrc1             = pdest;

            for (size_t colsub = 0; colsub < the16Mode; ++colsub)
            {
                const size_t col           = colsub * alignedLength;
                __m128i data               = _mm_load_si128((const __m128i*)(ptemSyndromArray + col));
                __m128i lowData            = _mm_shuffle_epi8(lowtable, _mm_and_si128(data, maskLow));
                __m128i highData           = _mm_srli_epi64(_mm_and_si128(data, maskHigh), 4);
                highData                   = _mm_shuffle_epi8(hightable, highData);
                data                       = _mm_xor_si128(lowData, highData);
                *pdest++                   = _mm_xor_si128(*psrc1++, data);
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

            const __m128i lowtable  = _mm_load_si128((const __m128i*)LowT[temp]);  // 低位表
            const __m128i hightable = _mm_load_si128((const __m128i*)HighT[temp]); // 高位表


            const gfExp* ptempOmegaArry    = omegaArry[i + errorCount];
            __m128i* pdest                 = (__m128i*)omegaValueArray;
            __m128i* psrc1                 = (__m128i*)omegaValueArray;

            for (size_t colsub = 0; colsub < the16Mode; ++colsub)
            {
                const size_t col           = colsub * alignedLength;
                __m128i data               = _mm_load_si128((const __m128i*)(ptempOmegaArry + col));
                __m128i lowData            = _mm_shuffle_epi8(lowtable, _mm_and_si128(data, maskLow));
                __m128i highData           = _mm_srli_epi64(_mm_and_si128(data, maskHigh), 4);
                highData                   = _mm_shuffle_epi8(hightable, highData);
                data                       = _mm_xor_si128(lowData, highData);
                *pdest++                   = _mm_xor_si128(*psrc1++, data);
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

        const __m128i omegaValueLowtable   = _mm_load_si128((const __m128i*)LowT[tempAlpha]);    // 低位表
        const __m128i omegaValueHightable  = _mm_load_si128((const __m128i*)HighT[tempAlpha]);   // 高位表

        const __m128i denominatorLowtable  = _mm_load_si128((const __m128i*)LowT[denominator]);  // 低位表
        const __m128i denominatorHightable = _mm_load_si128((const __m128i*)HighT[denominator]); // 高位表

        __m128i* pdest                     = (__m128i*)pDatas[errorSite];
        __m128i* psrc1                     = (__m128i*)pdest;

        for (size_t colsub = 0; colsub < the16Mode; ++colsub)
        {
            const size_t col = colsub * alignedLength;
            __m128i data     = _mm_load_si128((const __m128i*)(omegaValueArray + col));

            __m128i lowData  = _mm_shuffle_epi8(omegaValueLowtable, _mm_and_si128(data, maskLow));
            __m128i highData = _mm_srli_epi64(_mm_and_si128(data, maskHigh), 4);
            highData         = _mm_shuffle_epi8(omegaValueHightable, highData);

            data             = _mm_xor_si128(lowData, highData);

            lowData          = _mm_shuffle_epi8(denominatorLowtable, _mm_and_si128(data, maskLow));
            highData         = _mm_srli_epi64(_mm_and_si128(data, maskHigh), 4);
            highData         = _mm_shuffle_epi8(denominatorHightable, highData);

            data             = _mm_xor_si128(lowData, highData);
            *pdest++         = _mm_xor_si128(*psrc1++, data);
        }
    }

    FREE_ALIGNED(omegaValueArray);
    FREE_ALIGNED(omegaArry[0]);
    free(omegaArry);

    return 0;
}
// ------------------------------------------------------------------------------------------
//
static int decode1CheckSSE42Impl(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    static const size_t alignedLength = 16;
    const        size_t the16Mode     = count / alignedLength;

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
        __m128i* psrc1 = (__m128i*)pDatas[pSubList[i]];
        __m128i* psrc2 = (__m128i*)pDatas[pErInfo->m_pErrorLocationList[0]];
        __m128i* pdest = (__m128i*)pDatas[pErInfo->m_pErrorLocationList[0]];

        for (size_t col = 0; col < the16Mode; ++col)
        {
            *pdest++ = _mm_xor_si128(*psrc1++, *psrc2++);
        }
    }

    free(pSubList);

    return 0;
}

// ------------------------------------------------------------------------------------------
// 
int decodeSSE42(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    int result = 0;
    if (g_RS_K == 1)
    {
        result =  decode1CheckSSE42Impl(pDatas, pErInfo, count);
    }
    else
    {
        result = decodeSSE42Impl(pDatas, pErInfo, count);
    }

    return result;
}
// 
// ------------------------------------------------------------------------------------------
