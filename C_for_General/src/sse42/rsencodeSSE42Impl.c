/*******************************************************************************************
 *  @file      rsencodeSSE42Impl.c 2017\6\12 11:02:52 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "../rsencode.h"
#include "../auxiliary.h"
#include <string.h>
#include <nmmintrin.h>
// ------------------------------------------------------------------------------------------
// 
static int encodeSSE42Impl(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    const __m128i maskLow             = _mm_set1_epi8(0xf);  // 低位掩码
    const __m128i maskHigh            = _mm_set1_epi8(0xf0); // 高位掩码

    static const size_t alignedLength = 16;
    const        size_t the16Mode     = count / alignedLength;

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
            const __m128i lowtable  = _mm_load_si128((const __m128i*) LowT[pGenerator[g_RS_K][i + 1]]);  // 低位表
            const __m128i hightable = _mm_load_si128((const __m128i*)HighT[pGenerator[g_RS_K][i + 1]]);  // 高位表

            __m128i*     pdest      = (__m128i*)(checkSeg[i]);
            __m128i*     psrc1      = (__m128i*)(checkSeg[i + 1]);
            
            for (size_t colsub = 0; colsub < the16Mode; ++colsub)
            { // 按照16字节截取
                __m128i data     = _mm_load_si128((const __m128i*)(quotientArray + colsub * alignedLength));
                __m128i lowData  = _mm_shuffle_epi8(lowtable, _mm_and_si128(data, maskLow));
                __m128i highData = _mm_srli_epi64(_mm_and_si128(data, maskHigh), 4);
                highData         = _mm_shuffle_epi8(hightable, highData);
                data             = _mm_xor_si128(lowData, highData);
                *pdest++         = _mm_xor_si128(*psrc1++, data);
            }
        }

        const __m128i lowtable   = _mm_load_si128((const __m128i*)LowT[pGenerator[g_RS_K][g_RS_K]]);  // 低位表
        const __m128i hightable  = _mm_load_si128((const __m128i*)HighT[pGenerator[g_RS_K][g_RS_K]]); // 高位表

        __m128i* pdest           = (__m128i*)(checkSeg[g_RS_K - 1]);

        if (row < g_RS_N)
        {
            __m128i* psrc1       = (__m128i*)(dataSeg[row]);

            for (size_t colsub   = 0; colsub < the16Mode; ++colsub)
            {
                __m128i data     = _mm_load_si128((const __m128i*)(quotientArray + colsub * alignedLength));
                __m128i lowData  = _mm_shuffle_epi8(lowtable, _mm_and_si128(data, maskLow));
                __m128i highData = _mm_srli_epi64(_mm_and_si128(data, maskHigh), 4);
                highData         = _mm_shuffle_epi8(hightable, highData);
                data             = _mm_xor_si128(lowData, highData);
                *pdest++         = _mm_xor_si128(*psrc1++, data);
            }
        }
        else
        {
            for (size_t colsub = 0; colsub < the16Mode; ++colsub)
            {
                __m128i data       = _mm_load_si128((const __m128i*)(quotientArray + colsub * alignedLength));
                __m128i lowData    = _mm_shuffle_epi8(lowtable, _mm_and_si128(data, maskLow));
                __m128i highData   = _mm_srli_epi64(_mm_and_si128(data, maskHigh), 4);
                highData           = _mm_shuffle_epi8(hightable, highData);
                data               = _mm_xor_si128(lowData, highData);
                *pdest++           = data;
            }
        }
    }

    FREE_ALIGNED(quotientArray);

    return 0;
}


// ------------------------------------------------------------------------------------------
// 
static int encode1CheckSSE42Impl(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    static const size_t alignedLength = 16;
    const        size_t the32Mode     = count / alignedLength;

    memcpy(checkSeg[0], dataSeg[0], count);

    for (size_t i = 1; i < g_RS_N; ++i)
    {
        __m128i* psrc1  = (__m128i*)dataSeg[i];
        __m128i* psrc2  = (__m128i*)checkSeg[0];
        __m128i* pdest  = (__m128i*)checkSeg[0];

        for (size_t col = 0; col < the32Mode; ++col)
        {
            *pdest++ = _mm_xor_si128(*psrc1++, *psrc2++);
        }
    }

    return 0;
}

// ------------------------------------------------------------------------------------------
// 
int encodeSSE42(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    int result = 0;

    if (g_RS_K == 1)
    {
        result = encode1CheckSSE42Impl(dataSeg, checkSeg, count);
    }
    else
    {
        result = encodeSSE42Impl(dataSeg, checkSeg, count);
    }

    return result;
}
// 
// ------------------------------------------------------------------------------------------
