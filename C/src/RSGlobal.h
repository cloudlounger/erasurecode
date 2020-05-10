/*******************************************************************************
 *               Global.h defines the 全局变量
 * @file         Global.h 2013\08\30 17:23:30 $
 * @author       Wang Xiaotao<wangxiaotao1980@gmail.com>
 *******************************************************************************/

#ifndef GLOBAL_D5941620_1155_11E3_BA43_A41F7251BDE1_H__
#define GLOBAL_D5941620_1155_11E3_BA43_A41F7251BDE1_H__
/*******************************************************************************/
#if defined(__cplusplus)
extern "C"
{
#endif
extern int g_RS_N;                // 要做RS编码的数据长度
extern int g_RS_K;                // 做编码的RS编码校验段的长度
extern int g_RS_ALL;                // 数据段总长度

extern const unsigned char* g_RS_pGenerator;


#if defined(__cplusplus)
}
#endif

/*******************************************************************************/
#endif// GLOBAL_D5941620_1155_11E3_BA43_A41F7251BDE1_H__
