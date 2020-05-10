/*******************************************************************************************
 *  @file      rsdecodeAVX1Impl.c 2017\4\24 17:58:13 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "../rsdecode.h"
#include "../auxiliary.h"
#include <string.h>
#include <immintrin.h>
// ------------------------------------------------------------------------------------------
//
static int decodeAVXImpl(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    static const size_t alignedLength = 32;
    const __m128i maskLow             = _mm_set1_epi8(0xf);  // 低位掩码
    const __m128i maskHigh            = _mm_set1_epi8(0xf0); // 高位掩码
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
        __m256d* pdest = (__m256d*)syndromArray[g_RS_K - 1];
        __m256d* psrc1 = pdest;
        __m256d* psrc2 = (__m256d*)pDatas[j];

        for (size_t col = 0; col < the32Mode; ++col)
        {
            *pdest++ = _mm256_xor_pd(*psrc1++, *psrc2++);
        }
    }

    for (size_t i = 1; i < g_RS_K; ++i)
    {
        for (size_t j = 0; j < g_RS_ALL - 1; ++j)
        {
            const unsigned char expSub = AlphaT[i* (g_RS_ALL - 1 - j) % 255];

            const __m128i lowtable  = _mm_load_si128((const __m128i*) LowT[expSub]); // 低位表
            const __m128i hightable = _mm_load_si128((const __m128i*)HighT[expSub]); // 高位表

            const gfExp* pDataCol   = pDatas[j];
            __m256d* pdest          = (__m256d*)syndromArray[g_RS_K - 1 - i];
            __m256d* psrc1          = pdest;

            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                const size_t col           = colsub * alignedLength;
                __m256i data               = _mm256_load_si256((const __m256i*)(pDataCol + col));
                __m128i  firstHalfData     = _mm256_extractf128_si256(data, 0);
                __m128i  lowFirstHalfData  = _mm_and_si128(firstHalfData, maskLow);
                lowFirstHalfData           = _mm_shuffle_epi8(lowtable, lowFirstHalfData);
                __m128i  highFirstHalfData = _mm_and_si128(firstHalfData, maskHigh);
                highFirstHalfData          = _mm_srli_epi64(highFirstHalfData, 4);
                highFirstHalfData          = _mm_shuffle_epi8(hightable, highFirstHalfData);
                firstHalfData              = _mm_xor_si128(lowFirstHalfData, highFirstHalfData);
                __m128i  backHalfData      = _mm256_extractf128_si256(data, 1);
                __m128i  lowBackHalfData   = _mm_and_si128(backHalfData, maskLow);
                lowBackHalfData            = _mm_shuffle_epi8(lowtable, lowBackHalfData);
                __m128i highBackHalfData   = _mm_and_si128(backHalfData, maskHigh);
                highBackHalfData           = _mm_srli_epi64(highBackHalfData, 4);
                highBackHalfData           = _mm_shuffle_epi8(hightable, highBackHalfData);
                backHalfData               = _mm_xor_si128(lowBackHalfData, highBackHalfData);
                data                       = _mm256_insertf128_si256(_mm256_castsi128_si256(firstHalfData), backHalfData, 1);
                __m256d src2               = _mm256_castsi256_pd(data);
                *pdest++                   = _mm256_xor_pd(*psrc1++, src2);
            }
        }

        __m256d* pdest = (__m256d*)syndromArray[g_RS_K - 1 - i];
        __m256d* psrc1 = (__m256d*)syndromArray[g_RS_K - 1 - i];
        __m256d* psrc2 = (__m256d*)pDatas[g_RS_ALL - 1];

        for (size_t col = 0; col < the32Mode; ++col)
        {
            *pdest++ = _mm256_xor_pd(*psrc1++, *psrc2++);
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
        omegaArry[i] = omegaArry[0] + i *count;
    }

    gfExp* ppResult        = omegaArry[omeagLen - 1];
    gfExp* ppCurrentResult = 0;

    for (size_t i = errorCount + 1; i > 0; --i)
    {
        ppCurrentResult = ppResult;
        ppResult -= count;

        const __m128i lowtable  = _mm_load_si128((const __m128i*) LowT[pErInfo->m_pGamma[i - 1]]); // 低位表
        const __m128i hightable = _mm_load_si128((const __m128i*)HighT[pErInfo->m_pGamma[i - 1]]); // 高位表

        for (size_t j = g_RS_K; j > 0; --j)
        {
            const gfExp* ptemSyndromArray  = syndromArray[j - 1];
            __m256d*     pdest             = (__m256d*)ppCurrentResult;
            __m256d*     psrc1             = pdest;

            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                const size_t col           = colsub * alignedLength;
                __m256i data               = _mm256_load_si256((const __m256i*)(ptemSyndromArray + col));
                __m128i  firstHalfData     = _mm256_extractf128_si256(data, 0);
                __m128i  lowFirstHalfData  = _mm_and_si128(firstHalfData, maskLow);
                lowFirstHalfData           = _mm_shuffle_epi8(lowtable, lowFirstHalfData);
                __m128i  highFirstHalfData = _mm_and_si128(firstHalfData, maskHigh);
                highFirstHalfData          = _mm_srli_epi64(highFirstHalfData, 4);
                highFirstHalfData          = _mm_shuffle_epi8(hightable, highFirstHalfData);
                firstHalfData              = _mm_xor_si128(lowFirstHalfData, highFirstHalfData);
                __m128i  backHalfData      = _mm256_extractf128_si256(data, 1);
                __m128i  lowBackHalfData   = _mm_and_si128(backHalfData, maskLow);
                lowBackHalfData            = _mm_shuffle_epi8(lowtable, lowBackHalfData);
                __m128i highBackHalfData   = _mm_and_si128(backHalfData, maskHigh);
                highBackHalfData           = _mm_srli_epi64(highBackHalfData, 4);
                highBackHalfData           = _mm_shuffle_epi8(hightable, highBackHalfData);
                backHalfData               = _mm_xor_si128(lowBackHalfData, highBackHalfData);
                data                       = _mm256_insertf128_si256(_mm256_castsi128_si256(firstHalfData), backHalfData, 1);
                __m256d src2               = _mm256_castsi256_pd(data);
                *pdest++                   = _mm256_xor_pd(*psrc1++, src2);
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

            const __m128i lowtable         = _mm_load_si128((const __m128i*) LowT[temp]); // 低位表
            const __m128i hightable        = _mm_load_si128((const __m128i*)HighT[temp]); // 高位表
            const gfExp* ptempOmegaArry    = omegaArry[i + errorCount];
            __m256d* pdest                 = (__m256d*)omegaValueArray;
            __m256d* psrc1                 = (__m256d*)omegaValueArray;

            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                const size_t col           = colsub * alignedLength;
                __m256i data               = _mm256_load_si256((const __m256i*)(ptempOmegaArry + col));
                __m128i  firstHalfData     = _mm256_extractf128_si256(data, 0);
                __m128i  lowFirstHalfData  = _mm_and_si128(firstHalfData, maskLow);
                lowFirstHalfData           = _mm_shuffle_epi8(lowtable, lowFirstHalfData);
                __m128i  highFirstHalfData = _mm_and_si128(firstHalfData, maskHigh);
                highFirstHalfData          = _mm_srli_epi64(highFirstHalfData, 4);
                highFirstHalfData          = _mm_shuffle_epi8(hightable, highFirstHalfData);
                firstHalfData              = _mm_xor_si128(lowFirstHalfData, highFirstHalfData);

                __m128i  backHalfData      = _mm256_extractf128_si256(data, 1);
                __m128i  lowBackHalfData   = _mm_and_si128(backHalfData, maskLow);
                lowBackHalfData            = _mm_shuffle_epi8(lowtable, lowBackHalfData);
                __m128i highBackHalfData   = _mm_and_si128(backHalfData, maskHigh);
                highBackHalfData           = _mm_srli_epi64(highBackHalfData, 4);
                highBackHalfData           = _mm_shuffle_epi8(hightable, highBackHalfData);
                backHalfData               = _mm_xor_si128(lowBackHalfData, highBackHalfData);

                data                       = _mm256_insertf128_si256(_mm256_castsi128_si256(firstHalfData), backHalfData, 1);
                __m256d src2               = _mm256_castsi256_pd(data);
                *pdest++                   = _mm256_xor_pd(*psrc1++, src2);
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

        const gfExp tempAlpha                = AlphaT[g_RS_ALL - 1 - errorSite];
        const __m128i omegaValueLowtable     = _mm_load_si128((const __m128i*)LowT[tempAlpha]); // 低位表
        const __m128i omegaValueHightable    = _mm_load_si128((const __m128i*)HighT[tempAlpha]); // 高位表

        const __m128i denominatorLowtable    = _mm_load_si128((const __m128i*)LowT[denominator]);
        const __m128i denominatorHighowtable = _mm_load_si128((const __m128i*)HighT[denominator]);

        __m256d* pdest           = (__m256d*)pDatas[errorSite];
        __m256d* psrc1           = (__m256d*)pdest;

        for (size_t colsub = 0; colsub < the32Mode; ++colsub)
        {
            const size_t col = colsub * alignedLength;
            __m256i data               = _mm256_load_si256((const __m256i*)(omegaValueArray + col));

            __m128i  firstHalfData     = _mm256_extractf128_si256(data, 0);
            __m128i  lowFirstHalfData  = _mm_and_si128(firstHalfData, maskLow);
            lowFirstHalfData           = _mm_shuffle_epi8(omegaValueLowtable, lowFirstHalfData);
            __m128i  highFirstHalfData = _mm_and_si128(firstHalfData, maskHigh);
            highFirstHalfData          = _mm_srli_epi64(highFirstHalfData, 4);
            highFirstHalfData          = _mm_shuffle_epi8(omegaValueHightable, highFirstHalfData);
            firstHalfData              = _mm_xor_si128(lowFirstHalfData, highFirstHalfData);

            lowFirstHalfData           = _mm_and_si128(firstHalfData, maskLow);
            lowFirstHalfData           = _mm_shuffle_epi8(denominatorLowtable, lowFirstHalfData);
            highFirstHalfData          = _mm_and_si128(firstHalfData, maskHigh);
            highFirstHalfData          = _mm_srli_epi64(highFirstHalfData, 4);
            highFirstHalfData          = _mm_shuffle_epi8(denominatorHighowtable, highFirstHalfData);
            firstHalfData              = _mm_xor_si128(lowFirstHalfData, highFirstHalfData);

            __m128i  backHalfData      = _mm256_extractf128_si256(data, 1);
            __m128i  lowBackHalfData   = _mm_and_si128(backHalfData, maskLow);
            lowBackHalfData            = _mm_shuffle_epi8(omegaValueLowtable, lowBackHalfData);
            __m128i highBackHalfData   = _mm_and_si128(backHalfData, maskHigh);
            highBackHalfData           = _mm_srli_epi64(highBackHalfData, 4);
            highBackHalfData           = _mm_shuffle_epi8(omegaValueHightable, highBackHalfData);
            backHalfData               = _mm_xor_si128(lowBackHalfData, highBackHalfData);
            lowBackHalfData            = _mm_and_si128(backHalfData, maskLow);
            lowBackHalfData            = _mm_shuffle_epi8(denominatorLowtable, lowBackHalfData);
            highBackHalfData           = _mm_and_si128(backHalfData, maskHigh);
            highBackHalfData           = _mm_srli_epi64(highBackHalfData, 4);
            highBackHalfData           = _mm_shuffle_epi8(denominatorHighowtable, highBackHalfData);
            backHalfData               = _mm_xor_si128(lowBackHalfData, highBackHalfData);

            data                       = _mm256_insertf128_si256(_mm256_castsi128_si256(firstHalfData), backHalfData, 1);
            __m256d src2 = _mm256_castsi256_pd(data);
            *pdest++ = _mm256_xor_pd(*psrc1++, src2);
        }
    }

    FREE_ALIGNED(omegaValueArray);
    FREE_ALIGNED(omegaArry[0]);
    free(omegaArry);

    return 0;
}

// ------------------------------------------------------------------------------------------
//
static int decode1CheckAVXImpl(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
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
        __m256d* psrc1 = (__m256d*)pDatas[pSubList[i]];
        __m256d* psrc2 = (__m256d*)pDatas[pErInfo->m_pErrorLocationList[0]];
        __m256d* pdest = (__m256d*)pDatas[pErInfo->m_pErrorLocationList[0]];

        for (size_t col = 0; col < the32Mode; ++col)
        {
            *pdest++ = _mm256_xor_pd(*psrc1++, *psrc2++);
        }
    }

    free(pSubList);

    return 0;
}

// ------------------------------------------------------------------------------------------
// 
int decodeAVX(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count)
{
    int result = 0;
    if (g_RS_K == 1)
    {
        result =  decode1CheckAVXImpl(pDatas, pErInfo, count);
    }
    else
    {
        result = decodeAVXImpl(pDatas, pErInfo, count);
    }

    return result;
}
// 
// ------------------------------------------------------------------------------------------
