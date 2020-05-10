/*******************************************************************************************
 *  @file      x86cpu.h 2017\3\10 18:07:33 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *  @note      summary
 *******************************************************************************************/
#ifndef  X86CPU_54EBC38A_5E85_4B00_A3A9_25404293A423_H__
#define  X86CPU_54EBC38A_5E85_4B00_A3A9_25404293A423_H__

/*******************************************************************************************/

typedef enum  tagSIMDInstruction
{
    HW_UNKNOWN = 0,
    HW_SSE42   = 1,
    HW_AVX     = 2,
    HW_AVX2    = 3,
} SIMDInstruction;

#if defined(__cplusplus)
extern "C"
{
#endif

SIMDInstruction getSIMDInstruction();

#if defined(__cplusplus)
}
#endif

/*******************************************************************************************/
#endif// X86CPU_54EBC38A_5E85_4B00_A3A9_25404293A423_H__