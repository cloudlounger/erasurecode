/******************************************************************************* 
 *  @file      ec.cpp 2013\8\22 16:55:56 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 ******************************************************************************/

#include "../include/ec/ec.h"

#include "ReedSolomonEncode.h"
#include "ReedSolomonDecode.h"
/******************************************************************************/

namespace
{ 
    const unsigned int pwr                       =   8;
    const int g_generator_polynommial_index      =   0; // 多项式生成  使用

    GaloisField g_galoisFile(pwr, primitive_polynomial_power_8_1, primitive_polynomial_power_8_size);
    GaloisFieldPolynomial* g_pGeneratorPoly = 0;
    //GaloisFieldPolynomial g_generatorPoly(g_galoisFile);

    const ReedSolomonEncode* g_pReedSolomonEncode = 0;
    const ReedSolomonDecode* g_pReedSolomonDecode = 0;
}
// -----------------------------------------------------------------------------
// public   
int initializeEC( int n , int k )
{
    if ((n <= 0)||(k <= 0))
    {
        return -1;
    }

    if (n <= k)
    {
        return -2;
    }

    if ((n + k ) > 255)
    {
        return -3;
    }

    g_pGeneratorPoly = new GaloisFieldPolynomial(g_galoisFile);
    
    generatorPolynomialCreator(g_galoisFile, 
                               g_generator_polynommial_index,
                               k, 
                               *g_pGeneratorPoly);

    g_pReedSolomonEncode = new ReedSolomonEncode(n, k, g_galoisFile, *g_pGeneratorPoly);
    g_pReedSolomonDecode = new ReedSolomonDecode(n ,k, g_galoisFile, g_generator_polynommial_index);

    return 0;
}

// -----------------------------------------------------------------------------
// public   
int encodeEC( const unsigned char* pCode, unsigned char* pCheckCode )
{
    return g_pReedSolomonEncode->encode(pCode, pCheckCode) ? 0 : 1;
}

// -----------------------------------------------------------------------------
// public   
int decodeEC( unsigned char* pCode, const int* pLocationList, const int locationSize )
{
    return g_pReedSolomonDecode->decode(pCode, pLocationList, locationSize) ? 0 : 1;
}

// -----------------------------------------------------------------------------
// public   
void terminateEC()
{
    delete g_pReedSolomonEncode; 
    g_pReedSolomonEncode = 0;
    
    delete g_pReedSolomonDecode; 
    g_pReedSolomonDecode = 0;
    
    delete g_pGeneratorPoly;
    g_pGeneratorPoly = 0;
}



// 
// -----------------------------------------------------------------------------
