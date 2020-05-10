/*******************************************************************************************
 *  @file      rsencode.h 2017\4\6 15:00:15 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *  @note      summary
 *******************************************************************************************/
#ifndef  RSENCODE_19DEF068_4493_4F26_AA72_5B13C9483E6C_H__
#define  RSENCODE_19DEF068_4493_4F26_AA72_5B13C9483E6C_H__
#include "rs.h"

/*******************************************************************************************/
#if defined(__cplusplus)
extern "C"
{
#endif

    /*intel x86_64 AVX2实现*/
    int   encodeAVX2(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count);
    /*intel x86_64 AVX实现*/
    int   encodeAVX(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count);
    /*intel x86_64 sse42实现*/
    int   encodeSSE42(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count);
    /*intel x86_64 无simd实现*/
    int encodeNoSIMD(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count);
#if defined(__cplusplus)
}
#endif
/*******************************************************************************************/
#endif// RSENCODE_19DEF068_4493_4F26_AA72_5B13C9483E6C_H__