/*******************************************************************************************
 *  @file      rsdecode.h 2017\4\6 15:37:34 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *  @note      summary
 *******************************************************************************************/
#ifndef  RSDECODE_884C9EAC_4A98_4501_A675_66A9FAC369E8_H__
#define  RSDECODE_884C9EAC_4A98_4501_A675_66A9FAC369E8_H__

#include "rs.h"

/*******************************************************************************************/
#if defined(__cplusplus)
extern "C"
{
#endif
    /*intel x86_64 AVX2实现*/
    int decodeAVX2(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count);
    /*intel x86_64 AVX实现*/
    int decodeAVX(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count);
    /*intel x86_64 SSE42实现*/
    int decodeSSE42(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count);
    /*intel x86_64 无simd实现*/
    int decodeNoSIMD(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count);
#if defined(__cplusplus)
}
#endif
/*******************************************************************************************/
#endif// RSDECODE_884C9EAC_4A98_4501_A675_66A9FAC369E8_H__