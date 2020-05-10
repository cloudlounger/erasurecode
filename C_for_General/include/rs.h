/*******************************************************************************************
 *  @file      rs.h 2017\4\6 11:43:33 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *  @note      summary
 *******************************************************************************************/
#ifndef  RS_F4242881_5040_49FB_9456_0BCDD7C0C26A_H__
#define  RS_F4242881_5040_49FB_9456_0BCDD7C0C26A_H__

#include <stddef.h>

/*******************************************************************************************/
#if defined(__cplusplus)
extern "C"
{
#endif

    typedef unsigned char               gfExp;
    typedef struct tagErrorLocationInfo ErrorLocationInfo;
    typedef struct tagDataSeg
    {
        int         dataLen;
        gfExp**     dataSeg;
    } DataSeg;
    /**
     * 初始化编解码环境
     *
     * @param   size_t N  编码的数据块
     * @param   size_t K  解码的数据块
     * @return  int       0 为正确返回，非0为错误，由errorToStr可以得到信息
     */
    int initializeRS(size_t N, size_t K);

    /**
     * 返回初始化编解码环境中的设置的编码的数据块数
     *
     * @return  size_t 编码的数据块数
     */
    size_t getDataN();

    /**
     * 返回初始化编解码环境中的设置的解码的数据块数
     *
     * @return  size_t 解码的数据块数
     */
    size_t getCheckK();
    
    /**
     * 根据错误码，返回提示信息
     *
     * @param   int err        错误码
     * @return  const char*    提示信息字符串头指针
     */
    const char* errorToStr(int err);
    
    /**
     * N + K reed solomon编码
     *
     * @param   gfExp * dataSeg[](长度N)
     *          输入： 要编码的N个数据段，每个数据段的首指针放在数组dataSeg[N]里面.
     *                每个数据段要求为32字节的倍数，不足请补齐，用数据0填充.
     *                数组dataSeg[N]内的每个数据（数据段的首指针），要求32字节对齐，具体内存分配方式如下
     *                windows  : 分配为：_aligned_malloc(size_t, 32) 释放为：_aligned_free();
     *                linux    : 分配为：posix_memalign(void**, size_t, size_t) 释放为：free();
     *                Mac OS X : 分配为：posix_memalign(void**, size_t, size_t) 释放为：free();
     *
     * @param   gfExp * checkSeg[](长度K)
     *          输出： 编码编出的K个数据段，每个数据段的首指针放在数组checkSeg[K]里面.
     *                数据段内存由调用者分配好，此函数不负责内存管理.
     *                每个数据段要求为32字节的倍数，不足请补齐，用数据0填充.
     *                数组checkSeg[K]内的每个数据（数据段的首指针），要求32字节对齐，具体内存分配方式如下
     *                windows  : 分配为：_aligned_malloc(size_t, 32) 释放为：_aligned_free();
     *                linux    : 分配为：posix_memalign(void**, size_t, size_t) 释放为：free();
     *                Mac OS X : 分配为：posix_memalign(void**, size_t, size_t) 释放为：free();
     * @param   const size_t count
     *                  输入： 编解码的每个数据段的长度，单位字节（需要补齐32字节长度）
     * @return  int 返回0为正确计算，非0为错误，由errorToStr可以得到信息
     */
    int encodeRS(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count);
    
    /**
     * 根据错误位置信息，生成解码定位信息（解码使用）
     *
     * @param   const size_t* locationList
     *                  输入： 错误位置数组,每个数组元素在[0,N+K)范围内，升序
     * @param   const size_t locationLength
     *                  输入： 错误位置数组的长度
     *
     * @return  ErrorLocationInfo*   解码定位信息结构体指针
     */
    ErrorLocationInfo* createErrorLocationInfo(const size_t* locationList, const size_t locationLength);
    
    /**
     * 回收由createErrorLocationInfo创建的ErrorLocationInfo 的指针的
     *
     * @param   ErrorLocationInfo * errorInfo
     *                  输入： 要回收的ErrorLocationInfo指针
     *
     * @return  void
     */
    void deleteErrorLocationInfo(ErrorLocationInfo* errorInfo);

    /**
     * N + K reed solomon 解码
     *
     * @param   gfExp * pDatas[N + K](长度N+K)
     *          输入-输出： 要解码的32个数据段，每个数据段的首指针放在数组pDatas[]里面.
     *               其中输入输出混合在 pDatas[N + K]内，解码的下标为 createErrorLocationInfo 的第一个参数的错误列表
     *               每个数据段要求为32字节的倍数，不足请补齐，用数据0填充.
     *               数组pDatas[]内的每个数据（数据段的首指针），要求32字节对齐，具体内存分配方式如下
     *               windows  : 分配为：_aligned_malloc(size_t, 32) 释放为：_aligned_free();
     *               linux    : 分配为：posix_memalign(void**, size_t, size_t) 释放为：free();
     *               Mac OS X : 分配为：posix_memalign(void**, size_t, size_t) 释放为：free();
     * @param   const ErrorLocationInfo * pErInfo
     *          输入： 解码定位信息结构体指针
     * @param   const size_t count  
     *          输入： 编解码的每个数据段的长度，单位字节（需要补齐32字节长度）
     * @return  int 返回0为正确计算，非0为错误，由errorToStr可以得到信息
     */
    int decodeRS(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const size_t count);

    /**
     *  反初始化编解码环境, N与K都被初始化为0
     *
     * @return  void
     */
    void terminateRS();

#if defined(__cplusplus)
}
#endif

/*******************************************************************************************/
#endif// RS_F4242881_5040_49FB_9456_0BCDD7C0C26A_H__
