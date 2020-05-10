/*******************************************************************************************
 *  @file      x86cpu.c 2017\3\10 18:07:39 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "x86cpu.h"
#include <stdint.h>
// ------------------------------------------------------------------------------------------
// 
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
    #if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) \
        || defined(__GLIBC__)) && !defined(_CRAYC)                                    \
        || defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)

        #include <cpuid.h>
        static void getcpuid(int32_t out[4], int32_t x)
        {
            __cpuid_count(x, 0, out[0], out[1], out[2], out[3]);
        }


    #elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    
        #include <intrin.h>
        static void getcpuid(int32_t out[4], int32_t x)
        {
            __cpuidex(out, x, 0);
        }

    #else
        #error "Unknown platform"
    #endif
#else
    #error "No cpuid intrinsic defined for processor architecture."
#endif

// ------------------------------------------------------------------------------------------
// 
SIMDInstruction getSIMDInstruction()
{
    int32_t info[4];
    getcpuid(info, 0);
    int nIds = info[0];
    
    if (nIds >= 0x00000007)
    {
        getcpuid(info, 0x00000007);
        if ((info[1] & ((int32_t)1 << 5)) != 0)
        {
            return  HW_AVX2;
        }
    }

    if (nIds >= 0x00000001)
    {
        getcpuid(info, 0x00000001);
        if ((info[2] & ((int32_t)1 << 28)) != 0)
        {
            return HW_AVX;
        }

        if ((info[2] & ((int32_t)1 << 20)) != 0)
        {
            return HW_SSE42;
        }
    }

    return HW_UNKNOWN;

}


// 
// ------------------------------------------------------------------------------------------
