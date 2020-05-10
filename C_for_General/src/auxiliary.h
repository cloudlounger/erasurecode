/*******************************************************************************************
 *  @file      auxiliary.h 2017\4\6 13:03:05 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *  @note      summary
 *******************************************************************************************/
#ifndef  AUXILIARY_68CC1630_00B9_476A_8BA1_B61B2B5B2A0E_H__
#define  AUXILIARY_68CC1630_00B9_476A_8BA1_B61B2B5B2A0E_H__

#include <stddef.h>
#include "rs.h"
#include "config.h"

/*******************************************************************************************/
#if defined(__cplusplus)
extern "C"
{
#endif

// 要做RS编码的数据长度
extern size_t  g_RS_N;
// 做编码的RS编码校验段的长度
extern size_t  g_RS_K;
// 数据段总长度
extern size_t  g_RS_ALL;

struct tagErrorLocationInfo
{
    size_t     m_nErrorCount;              /* 错误长度*/
    size_t*    m_pErrorLocationList;       /* 错误位置列表           长度 为m_nErrorCount    */
    gfExp*     m_pGamma;                   /* Gamm多项式列表，       长度 为m_nErrorCount + 1*/
    gfExp*     m_pGammaDerivative;         /* Gamm求导后的多项式列表  长度 为m_nErrorCount    */
};

//伽罗华转换表
extern const ALIGNED_32 unsigned char  AlphaT[256];
extern const ALIGNED_32 unsigned char    InvT[256];
extern const ALIGNED_32 unsigned char    LowT[256][16];
extern const ALIGNED_32 unsigned char   HighT[256][16];

// 生成多项式(K最大值为16)
extern const unsigned char* const pGenerator[17];

#if defined(__cplusplus)
}
#endif
/*******************************************************************************************/
#endif// AUXILIARY_68CC1630_00B9_476A_8BA1_B61B2B5B2A0E_H__