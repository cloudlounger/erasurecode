/*******************************************************************************************
 *  @file      main.c 2017\4\10 19:26:14 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "rs.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define      s_dataLength    8
#define      s_checkLength   4
#define      s_allLength     (s_dataLength + s_checkLength)
#define      s_ArrayLenghth  1024 * 1024 * 16 + 32


int main(int argc, char* argv[])
{
    printf("this is librs demo\n");

    const size_t     s_errorLength  = 1;

    unsigned char* s_ppdata[s_allLength];


    int result = initializeRS(s_dataLength, s_checkLength);

    for (size_t i = 0; i < s_allLength; ++i)
    {

        result = posix_memalign((void**)(&s_ppdata[i]), 32, sizeof(char) * s_ArrayLenghth);
        
        for (size_t j = 0; j < s_ArrayLenghth; ++j)
        {
            unsigned char data = (unsigned char)(rand() % 256);
            s_ppdata[i][j] = data;
        }
    }

    result = encodeRS(s_ppdata, s_ppdata + s_dataLength, s_ArrayLenghth);



    for (size_t i = 0; i < s_allLength; ++i)
    {
        FREE_ALIGNED(s_ppdata[i]);
    }


    terminateRS();
    printf("run over\n");
    return 0;
}


// 
// ------------------------------------------------------------------------------------------
