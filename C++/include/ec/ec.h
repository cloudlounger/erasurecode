/*******************************************************************************
 *  @file      ec.h 2013\8\22 16:42:33 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com>(中文编码测试)
 
 ******************************************************************************/

#ifndef EC_FC99ACA3_2D10_4AC5_AAA5_039683B1F0C0_H__
#define EC_FC99ACA3_2D10_4AC5_AAA5_039683B1F0C0_H__
/******************************************************************************/
#if defined(__cplusplus)
extern "C" 
{
#endif

/**
 * 初始化EC环境 (要求如下 (n > 0) && (k > 0) && (n > k) && (n + k) <= 255
 *
 * @param   int n     ec要编码的长度 单位字节
 * @param   int k     ec校验码长度   单位字节
 * @return  int       正确返回 0
 */
int initializeEC(int n , int k);

/**
 * ec编码
 *
 * @param   const unsigned char * pCode    要编码的字符缓冲区 要求长度为 initializeEC 设定的 n
 * @param   unsigned char * pCheckCode     校验码的字符缓冲区 要求长度为 initializeEC 设定的 k
 * @return  int 正确编码返回 0， pCheckCode  指向的缓冲区被填写满校验码
 */
int encodeEC(const unsigned char* pCode, unsigned char* pCheckCode);
/**
 * ec解码 
 *
 * @param   unsigned char * pCode       待恢复的字符缓冲区 要求长度为 initializeEC 设定的(n + k)
 *                                      其中前n位为数据，后k位为校验码
 * @param   const int * pLocationList   错误位置列表， 里面数据范围为[0，n +k )
 * @param   const int locationSize      错误位置列表长度， 要求长度小于等于k
 * @return  int 正确解码返回 0            错误位置列表指出的错误位置被恢复
 */
int decodeEC(unsigned char* pCode, const int* pLocationList, const int locationSize);

/**
 * 
 * 反初始化EC环境 回收全局变量 
 * @return  void
 */
void terminateEC();


#if defined(__cplusplus)
}
#endif

/******************************************************************************/
#endif// EC_FC99ACA3_2D10_4AC5_AAA5_039683B1F0C0_H__