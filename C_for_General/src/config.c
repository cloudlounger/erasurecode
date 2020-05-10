/*******************************************************************************************
 *  @file      config.c 2017\3\8 12:41:24 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "config.h"
#include <errno.h>
// ------------------------------------------------------------------------------------------
// 

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

int posix_memalign(void **memptr, size_t alignment, size_t size)
{
    if ((alignment & -alignment) != alignment)
    {
        return EINVAL;
    }

    void* ptempmemptr = _aligned_malloc(size, alignment);
    if (NULL == ptempmemptr)
    {
        return ENOMEM;
    }
    else
    {
        *memptr = ptempmemptr;
        return  0;
    }
}

#endif// defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

// 
// ------------------------------------------------------------------------------------------
