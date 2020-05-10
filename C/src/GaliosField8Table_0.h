/*******************************************************************************
 *  @file      GaliosField8Table_0.h 2013\8\28 15:32:42 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com>(中文编码测试)
 *  根据本原多项式 x^8 + x^4 + x^3 + x^2 + 1 生成的伽罗华域空间置换、反置换，
 *  乘法求逆元，乘法、除法，指数表
 ******************************************************************************/

#ifndef GALIOSFIELD8TABLE_0_7C9C4DFA_8E40_4C13_9928_64C50FA2D3FE_H__
#define GALIOSFIELD8TABLE_0_7C9C4DFA_8E40_4C13_9928_64C50FA2D3FE_H__
/******************************************************************************/
#if defined(__cplusplus)
extern "C"
{
#endif

extern const unsigned char g_galoisField8AlphaTable[256];
extern const unsigned char g_galoisField8MultiplyTable[256][256];
extern const unsigned char g_galoisField8DivideTable  [256][256];
extern const unsigned char g_galoisField8ExponentTable[256][256];
 
#if defined(__cplusplus)
}
#endif
/******************************************************************************/
#endif// GALIOSFIELD8TABLE_0_7C9C4DFA_8E40_4C13_9928_64C50FA2D3FE_H__