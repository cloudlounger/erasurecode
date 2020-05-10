/*******************************************************************************************
 *  @file      config.h 2016\12\22 12:23:27 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *  @note      summary
 *******************************************************************************************/
#ifndef  CONFIG_D5F55B67_41CD_49EA_AE93_9FE8A4E05C20_H__
#define  CONFIG_D5F55B67_41CD_49EA_AE93_9FE8A4E05C20_H__
/*******************************************************************************************/

#include <stdlib.h>

#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
    
    #define  ALIGNED_32             __attribute__((aligned(32)))
    #define  FREE_ALIGNED(p)        free(p)

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#if defined(__cplusplus)
extern "C"
{
#endif
    int posix_memalign(void **memptr, size_t alignment, size_t size);
#if defined(__cplusplus)
}
#endif

    #define  ALIGNED_32             __declspec(align(32))
    #define  FREE_ALIGNED(p)        _aligned_free(p)
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    
    #define  ALIGNED_32             __attribute__((aligned(32)))
    #define  FREE_ALIGNED(p)        free(p)

#else

    #error "Unknown platform"

#endif


/*******************************************************************************************/
#endif// CONFIG_D5F55B67_41CD_49EA_AE93_9FE8A4E05C20_H__
