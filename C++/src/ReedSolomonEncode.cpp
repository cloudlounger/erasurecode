/******************************************************************************* 
 *  @file      ReedSolomonEncode.cpp 2013\8\19 16:07:02 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 ******************************************************************************/

#include "ReedSolomonEncode.h"
/******************************************************************************/


// -----------------------------------------------------------------------------
// public   
ReedSolomonEncode::ReedSolomonEncode( const std::size_t datalength, 
                                      const std::size_t checklenth, 
                                      const GaloisField& gfield, 
                                      const GaloisFieldPolynomial& generatePoly)
     : m_code_length(datalength + checklenth)
     , m_data_length(datalength)
     , m_fec_length(checklenth)
     , m_gfield(gfield)
     , m_generatorPoly(generatePoly)
{

}
// -----------------------------------------------------------------------------
//  ReedSolomonEncode: Public, Destructor

ReedSolomonEncode::~ReedSolomonEncode()
{

}

// -----------------------------------------------------------------------------
// public   
bool ReedSolomonEncode::encode( const unsigned char* data, unsigned char* fecData ) const
{
    GaloisFieldPolynomial gpoly(m_gfield, m_code_length - 1);

    for(std::size_t i = 0; i < m_data_length; ++i)
    {
        gpoly[gpoly.deg() - i] = GaloisFieldElement(m_gfield, static_cast<field_symbol>(data[i]));
    }

    GaloisFieldPolynomial remainder = gpoly % m_generatorPoly;

    if (remainder.deg() == (m_fec_length - 1))
    {
        for (std::size_t i = 0; i < m_fec_length; ++i)
        {
            fecData[i] = static_cast<unsigned char>(remainder[remainder.deg() - i].poly()  & m_gfield.mask());
        }
        return true;
    }
    else
    {
        return false;
    }
}
// 
// -----------------------------------------------------------------------------
