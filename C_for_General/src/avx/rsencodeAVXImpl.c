/*******************************************************************************************
 *  @file      rsencodeAVX1Impl.c 2017\4\24 17:58:35 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "../rsencode.h"
#include "../auxiliary.h"
#include <string.h>
#include <immintrin.h>

// ------------------------------------------------------------------------------------------
// 
static int encodeAVX1Impl(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    const __m128i maskLow             = _mm_set1_epi8(0xf);  // 低位掩码
    const __m128i maskHigh            = _mm_set1_epi8(0xf0); // 高位掩码
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
            const __m128i lowtable  = _mm_load_si128((const __m128i*) LowT[pGenerator[g_RS_K][i + 1]]);  // 低位表
            const __m128i hightable = _mm_load_si128((const __m128i*)HighT[pGenerator[g_RS_K][i + 1]]); // 高位表

            __m256d*     pdest = (__m256d*)(checkSeg[i]);
            __m256d*     psrc1 = (__m256d*)(checkSeg[i + 1]);
            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            { // 按照32字节截取, 转换需要按照16字节截取

                __m256i data  = _mm256_load_si256((const __m256i*)(quotientArray + colsub * alignedLength));

                //计算前16字节
                //计算低位乘法
                __m128i  firstHalfData      = _mm256_extractf128_si256(data, 0);
                __m128i  lowFirstHalfData   = _mm_and_si128(firstHalfData, maskLow);
                lowFirstHalfData            = _mm_shuffle_epi8(lowtable, lowFirstHalfData);
                //计算高位乘法
                __m128i  highFirstHalfData  = _mm_and_si128(firstHalfData, maskHigh);
                highFirstHalfData           = _mm_srli_epi64(highFirstHalfData, 4);
                highFirstHalfData           = _mm_shuffle_epi8(hightable, highFirstHalfData);
                // 最终结果 低位 ^ 高位
                firstHalfData               = _mm_xor_si128(lowFirstHalfData, highFirstHalfData); //firstHalfData这存储了低16字节结果

                //计算后16字节
                //计算低位乘法
                __m128i  backHalfData       = _mm256_extractf128_si256(data, 1);
                __m128i  lowBackHalfData    = _mm_and_si128(backHalfData, maskLow);
                lowBackHalfData             = _mm_shuffle_epi8(lowtable, lowBackHalfData);
                //计算高位乘法
                __m128i highBackHalfData    = _mm_and_si128(backHalfData, maskHigh);
                highBackHalfData            = _mm_srli_epi64(highBackHalfData, 4);
                highBackHalfData            = _mm_shuffle_epi8(hightable, highBackHalfData);
                // 最终结果 低位 ^ 高位
                backHalfData                = _mm_xor_si128(lowBackHalfData, highBackHalfData); //backHalfData这存储了高16字节结果

                data                        = _mm256_insertf128_si256(_mm256_castsi128_si256(firstHalfData), backHalfData, 1);
                __m256d src2                = _mm256_castsi256_pd(data);
                *pdest++                    = _mm256_xor_pd(*psrc1++, src2);

            }
        }

        const __m128i lowtable  = _mm_load_si128((const __m128i*) LowT[pGenerator[g_RS_K][g_RS_K]]); // 低位表
        const __m128i hightable = _mm_load_si128((const __m128i*)HighT[pGenerator[g_RS_K][g_RS_K]]); // 高位表
        __m256d* pdest = (__m256d*)(checkSeg[g_RS_K - 1]);

        if (row < g_RS_N)
        {
            __m256d* psrc1 = (__m256d*)(dataSeg[row]);

            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                __m256i data  = _mm256_load_si256((const __m256i*)(quotientArray + colsub * alignedLength));

                //计算前16字节
                //计算低位乘法
                __m128i  firstHalfData      = _mm256_extractf128_si256(data, 0);
                __m128i  lowFirstHalfData   = _mm_and_si128(firstHalfData, maskLow);
                lowFirstHalfData            = _mm_shuffle_epi8(lowtable, lowFirstHalfData);
                //计算高位乘法
                __m128i  highFirstHalfData  = _mm_and_si128(firstHalfData, maskHigh);
                highFirstHalfData           = _mm_srli_epi64(highFirstHalfData, 4);
                highFirstHalfData           = _mm_shuffle_epi8(hightable, highFirstHalfData);
                // 最终结果 低位 ^ 高位
                firstHalfData               = _mm_xor_si128(lowFirstHalfData, highFirstHalfData); //firstHalfData这存储了低16字节结果
                //计算后16字节
                //计算低位乘法
                __m128i  backHalfData       = _mm256_extractf128_si256(data, 1);
                __m128i  lowBackHalfData    = _mm_and_si128(backHalfData, maskLow);
                lowBackHalfData             = _mm_shuffle_epi8(lowtable, lowBackHalfData);
                //计算高位乘法
                __m128i highBackHalfData    = _mm_and_si128(backHalfData, maskHigh);
                highBackHalfData            = _mm_srli_epi64(highBackHalfData, 4);
                highBackHalfData            = _mm_shuffle_epi8(hightable, highBackHalfData);
                // 最终结果 低位 ^ 高位
                backHalfData                = _mm_xor_si128(lowBackHalfData, highBackHalfData); //backHalfData这存储了高16字节结果

                data                        = _mm256_insertf128_si256(_mm256_castsi128_si256(firstHalfData), backHalfData, 1);
                __m256d src2                = _mm256_castsi256_pd(data);
                *pdest++                    = _mm256_xor_pd(*psrc1++, src2);
            }
        }
        else
        {
            for (size_t colsub = 0; colsub < the32Mode; ++colsub)
            {
                __m256i data  = _mm256_load_si256((const __m256i*)(quotientArray + colsub * alignedLength));

                //计算前16字节
                //计算低位乘法
                __m128i  firstHalfData      = _mm256_extractf128_si256(data, 0);                // 提取前16字节

                __m128i  lowFirstHalfData   = _mm_and_si128(firstHalfData, maskLow);            // 提取每一字节的低4位
                lowFirstHalfData            = _mm_shuffle_epi8(lowtable, lowFirstHalfData);     // 根据迦洛华域低乘法表转换原来数据
                //计算高位乘法
                __m128i  highFirstHalfData  = _mm_and_si128(firstHalfData, maskHigh);           // 提取每一字节的高4位
                highFirstHalfData           = _mm_srli_epi64(highFirstHalfData, 4);             // 高4位右移动4位到低位
                highFirstHalfData           = _mm_shuffle_epi8(hightable, highFirstHalfData);   // 根据迦洛华域高乘法表转换原来数据

                firstHalfData               = _mm_xor_si128(lowFirstHalfData, highFirstHalfData); // 最终结果 低位 ^ 高位,
                // firstHalfData这存储了低16字节结果
                //计算后16字节
                //计算低位乘法
                __m128i  backHalfData       = _mm256_extractf128_si256(data, 1);
                __m128i  lowBackHalfData    = _mm_and_si128(backHalfData, maskLow);
                lowBackHalfData             = _mm_shuffle_epi8(lowtable, lowBackHalfData);
                //计算高位乘法
                __m128i highBackHalfData    = _mm_and_si128(backHalfData, maskHigh);
                highBackHalfData            = _mm_srli_epi64(highBackHalfData, 4);
                highBackHalfData            = _mm_shuffle_epi8(hightable, highBackHalfData);
                // 最终结果 低位 ^ 高位
                backHalfData                = _mm_xor_si128(lowBackHalfData, highBackHalfData); //backHalfData这存储了高16字节结果

                data                        = _mm256_insertf128_si256(_mm256_castsi128_si256(firstHalfData), backHalfData, 1);
                __m256d src2                = _mm256_castsi256_pd(data);
                *pdest++                    = src2;
            }
        }
    }

    FREE_ALIGNED(quotientArray);

    return 0;

}

// ------------------------------------------------------------------------------------------
// 
static int encode1CheckAVX1Impl(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    static const size_t alignedLength = 32;
    const        size_t the32Mode     = count / alignedLength;

    memcpy(checkSeg[0], dataSeg[0], count);

    for (size_t i = 1; i < g_RS_N; ++i)
    {
        __m256d* psrc1  = (__m256d*)dataSeg[i];
        __m256d* psrc2  = (__m256d*)checkSeg[0];
        __m256d* pdest  = (__m256d*)checkSeg[0];

        for (size_t col = 0; col < the32Mode; ++col)
        {
            *pdest++ = _mm256_xor_pd(*psrc1++, *psrc2++);
        }
    }

    return 0;
}

// ------------------------------------------------------------------------------------------
// 
int encodeAVX(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    int result = 0;

    if (g_RS_K == 1)
    {
        result = encode1CheckAVX1Impl(dataSeg, checkSeg, count);
    }
    else
    {
        result = encodeAVX1Impl(dataSeg, checkSeg, count);
    }

    return result;
}
// 
// ------------------------------------------------------------------------------------------
