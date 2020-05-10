/*******************************************************************************************
 *  @file      rsencode.c 2017\4\6 13:58:13 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 *******************************************************************************************/
#include "rsencode.h"
#include "auxiliary.h"
#include <string.h>

// ------------------------------------------------------------------------------------------
// 
static int encodeNoSIMDImpl(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    for (size_t i = 0; i < g_RS_K; ++i)
    {
        memcpy(checkSeg[i], dataSeg[i], count);
    }

    gfExp* quotientArray = (gfExp*)malloc(sizeof(gfExp) * count);

    for (size_t row = g_RS_K; row < g_RS_ALL; ++row)
    {
        memcpy(quotientArray, checkSeg[0], count);

        for (int i = 0; i < (g_RS_K - 1); ++i)
        {

            gfExp*       pdest      = checkSeg[i];
            gfExp*       psrc1      = checkSeg[i + 1];
            const gfExp* lowTable   = LowT[pGenerator[g_RS_K][i + 1]];
            const gfExp* highTable  = HighT[pGenerator[g_RS_K][i + 1]];

            for (size_t col = 0; col < count; ++col)
            {
                gfExp sub  = quotientArray[col];
                pdest[col] = psrc1[col] ^ lowTable[sub & 0xf] ^ highTable[(sub & 0xf0) >> 4];
            }
        }

        gfExp* pdest            =      checkSeg[g_RS_K - 1];
        const gfExp* lowTable   =  LowT[pGenerator[g_RS_K][g_RS_K]];
        const gfExp* highTable  =  HighT[pGenerator[g_RS_K][g_RS_K]];
        if (row < g_RS_N)
        {
            gfExp*     psrc1      = dataSeg[row];
            for (size_t col = 0; col < count; ++col)
            {
                gfExp sub  = quotientArray[col];
                pdest[col] = psrc1[col] ^ lowTable[sub & 0xf] ^ highTable[(sub & 0xf0) >> 4];
            }
        }
        else
        {
            for (size_t col = 0; col < count; ++col)
            {
                gfExp sub  = quotientArray[col];
                pdest[col] = lowTable[sub & 0xf] ^ highTable[(sub & 0xf0) >> 4];
            }
        }
    }

    free(quotientArray);

    return 0;
}

// ------------------------------------------------------------------------------------------
//
static int encode1CheckNoSIMDImpl(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    gfExp* pdest = checkSeg[0];

    memcpy(pdest, dataSeg[0], count);

    for (size_t i = 1; i < g_RS_N; ++i)
    {
        gfExp* psrc  = dataSeg[i];
        for (size_t col = 0; col < count; ++col)
        {
            pdest[col] ^= psrc[col];
        }
    }

    return 0;
}


int encodeNoSIMD(gfExp* dataSeg[], gfExp* checkSeg[], const size_t count)
{
    int result = 0;

    if (g_RS_K == 1)
    {
        result = encode1CheckNoSIMDImpl(dataSeg, checkSeg, count);
    }
    else
    {
        result = encodeNoSIMDImpl(dataSeg, checkSeg, count);
    }

    return result;
}



// 
// ------------------------------------------------------------------------------------------
