/*******************************************************************************************
 *  @file      rsencodeAVX2Impl.c 2017\4\24 11:39:58 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "../rsencode.h"
#include "../auxiliary.h"
#include <string.h>
#include <immintrin.h>
// ------------------------------------------------------------------------------------------
// 
static int encodeAVX2Impl(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    const __m256i maskLow             = _mm256_set1_epi8(0xf);  // 低位掩码
    const __m256i maskHigh            = _mm256_set1_epi8(0xf0); // 高位掩码

    static const size_t alignedLength = 32;
    const        size_t the32Mode     = count / alignedLength;

    for (int i = 0; i < g_RS_K; ++i)
    {
        memcpy(checkSeg[i], dataSeg[i], count);
    }

    gfExp* quotientArray = NULL;
    if (0 != posix_memalign((void**)(&quotientArray), alignedLength, sizeof(gfExp) * count))
    {
        return -1;
    }

    for (size_t row = g_RS_K; row < g_RS_ALL; ++row)
    {
        memcpy(quotientArray, checkSeg[0], count);

        for (int i = 0; i < (g_RS_K - 1); ++i)
        {
            const __m256i lowtable  = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*) LowT[pGenerator[g_RS_K][i + 1]]));  // 低位表
            const __m256i hightable = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)HighT[pGenerator[g_RS_K][i + 1]])); // 高位表

            __m256i*     pdest      = (__m256i*)(checkSeg[i]);
            __m256i*     psrc1      = (__m256i*)(checkSeg[i + 1]);

            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            { //
                __m256i data       = _mm256_load_si256((const __m256i*)(quotientArray + colsub * alignedLength));
                __m256i lowData    = _mm256_shuffle_epi8(lowtable, _mm256_and_si256(data, maskLow));
                __m256i highData   = _mm256_srli_epi64(_mm256_and_si256(data, maskHigh), 4);
                highData           = _mm256_shuffle_epi8(hightable, highData);
                data               = _mm256_xor_si256(lowData, highData);
                *pdest++           = _mm256_xor_si256(*psrc1++, data);
            }
        }

        const __m256i lowtable  = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)LowT[pGenerator[g_RS_K][g_RS_K]]));  // 低位表
        const __m256i hightable = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i*)HighT[pGenerator[g_RS_K][g_RS_K]])); // 高位表

        __m256i* pdest = (__m256i*)(checkSeg[g_RS_K - 1]);

        if (row < g_RS_N)
        {
            __m256i* psrc1 = (__m256i*)(dataSeg[row]);

            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                __m256i data       = _mm256_load_si256((const __m256i*)(quotientArray + colsub * alignedLength));
                __m256i lowData    = _mm256_shuffle_epi8(lowtable, _mm256_and_si256(data, maskLow));
                __m256i highData   = _mm256_srli_epi64(_mm256_and_si256(data, maskHigh), 4);
                highData           = _mm256_shuffle_epi8(hightable, highData);
                data               = _mm256_xor_si256(lowData, highData);
                *pdest++           = _mm256_xor_si256(*psrc1++, data);
            }
        }
        else
        {
            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                __m256i data       = _mm256_load_si256((const __m256i*)(quotientArray + colsub * alignedLength));
                __m256i lowData    = _mm256_shuffle_epi8(lowtable, _mm256_and_si256(data, maskLow));
                __m256i highData   = _mm256_srli_epi64(_mm256_and_si256(data, maskHigh), 4);
                highData           = _mm256_shuffle_epi8(hightable, highData);
                data               = _mm256_xor_si256(lowData, highData);
                *pdest++           = data;
            }
        }
    }

    FREE_ALIGNED(quotientArray);

    return 0;
}


// ------------------------------------------------------------------------------------------
// 
static int encode1CheckAVX2Impl(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    static const size_t alignedLength = 32;
    const        size_t the32Mode     = count / alignedLength;

    memcpy(checkSeg[0], dataSeg[0], count);

    for (size_t i = 1; i < g_RS_N; ++i)
    {
        __m256i* psrc1  = (__m256i*)dataSeg[i];
        __m256i* psrc2  = (__m256i*)checkSeg[0];
        __m256i* pdest  = (__m256i*)checkSeg[0];

        for (size_t col = 0; col < the32Mode; ++col)
        {
            *pdest++ = _mm256_xor_si256(*psrc1++, *psrc2++);
        }
    }

    return 0;
}

// ------------------------------------------------------------------------------------------
// 
int encodeAVX2(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    int result = 0;

    if (g_RS_K == 1)
    {
        result = encode1CheckAVX2Impl(dataSeg, checkSeg, count);
    }
    else
    {
        result = encodeAVX2Impl(dataSeg, checkSeg, count);
    }

    return result;
}

// 
// ------------------------------------------------------------------------------------------
