#include "ReedSolomonDecode.h"

/******************************************************************************/

// -----------------------------------------------------------------------------
// public   
ReedSolomonDecode::ReedSolomonDecode( const std::size_t datalength, 
                                      const std::size_t checklenth, 
                                      const GaloisField& gfield,
                                      const unsigned int gen_initial_index )
    : m_code_length(datalength + checklenth)
    , m_data_length(datalength)
    , m_fec_length(checklenth)
    , m_gfield(gfield)
    , m_rootExponentTable(gfield.size() + 1, 0)
    , m_syndromeExponentTable(checklenth, 0)
    , m_gammaTable(gfield.size() + 1, GaloisFieldPolynomial(m_gfield))
    , m_XValue(generateXValue(gfield))
    , m_genInitialIndex(gen_initial_index)
{
    init();
}
// -----------------------------------------------------------------------------
//  ReedSolomonDecode: Public, Destructor

ReedSolomonDecode::~ReedSolomonDecode()
{


}


// -----------------------------------------------------------------------------
// public   
bool ReedSolomonDecode::checkData(const unsigned char* pData) const
{
    GaloisFieldPolynomial received(m_gfield, m_code_length - 1);
    loadData(received, pData);

    GaloisFieldPolynomial syndrome(m_gfield);

    return (computeSyndrome(received, syndrome) == 0) ? true : false;

}

// -----------------------------------------------------------------------------
// public   
bool ReedSolomonDecode::decode( unsigned char* pData, 
                                const int* locationList, 
                                const int locationLength ) const
{
    if (locationLength > m_fec_length)
    {
        return false;
    }

    if (locationLength == 0)
    {
        return true;
    }
    int* pChangeLocation = changeLocation(locationList, locationLength);

    GaloisFieldPolynomial received(m_gfield, m_code_length - 1);

    loadData(received, pData);

    GaloisFieldPolynomial syndrome(m_gfield);

    if (computeSyndrome(received, syndrome) == 0)
    {
        delete []pChangeLocation;
        return true;
    }

    GaloisFieldPolynomial lambda(m_gfield);

    computeGamma(lambda, pChangeLocation, locationLength);

    if (locationLength < m_fec_length)
    {
        modifiedBerlekampMasseyAlgorithm(lambda,syndrome, locationLength);
    }


    int computerLength =  ChienSearchRoots(lambda,pChangeLocation);

     if (0 == computerLength)
     {
        /*
            Syndrome （状态多项式）非0， 但是仍旧没有得到任何错误定位，
            其原因如下：
            编码数据的错误值过多，超过算法所能侦测和定位的限制。
        */
        delete []pChangeLocation;
        return false;
    }
    else if ((2 * computerLength - locationLength) > m_fec_length)
    {
        /*
            过多 侦测到错误位置 和 已知错误位置， 不满足：2E + S <= m_fec_length
            E : 侦测到错误个数
            S : 已经知道的错误个数
        */
        delete []pChangeLocation;
        return false;
    }

     bool bResult = forneyAlgorithm(pChangeLocation, computerLength, lambda, syndrome, pData);
     delete []pChangeLocation;

     return bResult;

}



// -----------------------------------------------------------------------------
// protected   
void ReedSolomonDecode::init()
{

    // 注意这里的更改，简化求值步骤，就有可能在这里
    for (std::size_t i = 0; i < m_gfield.size() + 1; ++i)
    {
        m_rootExponentTable[i] = m_gfield.exp(m_gfield.alpha(m_gfield.size() - i),(1 - m_genInitialIndex));
    }

    for (std::size_t i = 0; i < m_fec_length; ++i)
    {
        m_syndromeExponentTable[i] = m_gfield.alpha(m_genInitialIndex + i);
    }


    for (std::size_t i = 0; i < m_gfield.size() + 1; ++i)
    {
        m_gammaTable[i] = (1 + (m_XValue * GaloisFieldElement(m_gfield, m_gfield.alpha(i))));
    }
}

// -----------------------------------------------------------------------------
// protected   
int ReedSolomonDecode::computeSyndrome( const GaloisFieldPolynomial& received, 
                                              GaloisFieldPolynomial& syndrome ) const
{
    int error_flag = 0;
    syndrome = GaloisFieldPolynomial(m_gfield,m_fec_length - 1);
    for (std::size_t i = 0; i < m_fec_length; ++i)
    {
        syndrome[i]  = received(m_syndromeExponentTable[i]);
        error_flag  |= syndrome[i].poly();
    }
    return error_flag;
}

// -----------------------------------------------------------------------------
// protected   
void ReedSolomonDecode::loadData( GaloisFieldPolynomial& received, 
                                  const unsigned char* pData ) const
{
    for (std::size_t i = 0; i < m_code_length; ++i)
    {
        received[i] = pData[m_code_length - i - 1];
    }
}

// -----------------------------------------------------------------------------
// protected   
void ReedSolomonDecode::computeGamma( GaloisFieldPolynomial& gamma, 
                                       const int* erasure_locations, 
                                       const int locationLenth ) const
{
    gamma = GaloisFieldElement(m_gfield, 1);
    for (int i = 0; i < locationLenth; ++i)
    {
        gamma *= m_gammaTable[erasure_locations[i]];
    }
    gamma.simplify();
}

// -----------------------------------------------------------------------------
// protected   
void ReedSolomonDecode::computeDiscrepancy( GaloisFieldElement& discrepancy, 
                                             const GaloisFieldPolynomial& lambda, 
                                             const GaloisFieldPolynomial& syndrome, 
                                             const std::size_t& l, 
                                             const std::size_t& round ) const
{
   /*
    在本次的BMA（Berlekamp-Massey Algorithm）计算的lambda多项式差异
    */
    int upper_bound = std::min(static_cast<int>(l),lambda.deg());
    discrepancy = 0;
    for (int i = 0; i <= upper_bound; ++i)
    {
        discrepancy += lambda[i] * syndrome[round - i];
    }
}

// -----------------------------------------------------------------------------
// protected   
void ReedSolomonDecode::modifiedBerlekampMasseyAlgorithm( GaloisFieldPolynomial&       lambda, 
                                                             const GaloisFieldPolynomial& syndrome, 
                                                             const std::size_t       erasure_count ) const
{
    /*
        改进的Berlekamp-Massey算法
        通过标识最短长度的线性反馈回移位寄存器(the shortest length linear feed-back shift register )（LFSR）
        生成等价于 Syndrome （状态多项式） 的 序列。
    */

    int i = -1;
    std::size_t l = erasure_count;

    GaloisFieldElement discrepancy(m_gfield,0);

    GaloisFieldPolynomial previous_lambda = lambda << 1;

    for (std::size_t round = erasure_count; round < m_fec_length; ++round)
    {
        computeDiscrepancy(discrepancy,lambda,syndrome,l,round);

        if (discrepancy != 0)
        {
            GaloisFieldPolynomial tau = lambda - discrepancy * previous_lambda;

            if (static_cast<int>(l) < (static_cast<int>(round) - i))
            {
                std::size_t tmp = round - i;
                i = static_cast<int>(round - l);
                l = tmp;
                previous_lambda = lambda / discrepancy;
            }
            lambda = tau;
        }
        previous_lambda <<= 1;
    }
}

// -----------------------------------------------------------------------------
// protected   
bool ReedSolomonDecode::forneyAlgorithm( const int * error_locations, 
                                          const int locationlength, 
                                          const GaloisFieldPolynomial& lambda, 
                                          const GaloisFieldPolynomial& syndrome, 
                                          unsigned char* pData ) const
{
    // Forney 算法，用于计算错误位置的值
    
    GaloisFieldPolynomial omega = (lambda * syndrome) % m_fec_length;
    GaloisFieldPolynomial lambda_derivative = lambda.derivative();

    GaloisFieldElement numerator(m_gfield);
    GaloisFieldElement denominator(m_gfield);

    for (std::size_t i = 0; i < locationlength; ++i)
    {
        int error_location                  = error_locations[i];
        field_symbol  alpha_inverse         = m_gfield.alpha(error_location);
        numerator                           = (omega(alpha_inverse) * m_rootExponentTable[error_location]);
        denominator                         = lambda_derivative(alpha_inverse);
        if (numerator != 0)
        {
            if (denominator != 0)
            {
                
                pData[error_location - (m_gfield.size() - m_code_length) - 1] 
                                    ^= m_gfield.div(numerator.poly(),denominator.poly());
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// protected   
int ReedSolomonDecode::ChienSearchRoots( const GaloisFieldPolynomial& poly, int* root_list ) const
{
    /*
     *   Chien 搜索 ： 通过枚举搜索所有在给定的非零元素有限域来查找错误位置多项式的根
     */
 
    std::size_t polynomial_degree = poly.deg();
    int root_list_size = 0;
    for (int i = 1; i <= static_cast<int>(m_gfield.size()); ++i)
    {
        if (0 == poly(m_gfield.alpha(i)).poly())
        {
            root_list[root_list_size] = i;
            root_list_size++;
            if (root_list_size == polynomial_degree)
            {
                break;
            }
        }
    }
    return root_list_size;
}

// -----------------------------------------------------------------------------
// private   
int* ReedSolomonDecode::changeLocation( const int* locationList, const int  locationLength) const
{
    int* pResult = new int[m_fec_length];
    for (int i = 0; i < locationLength; ++i)
    {
        pResult[i] = m_code_length - 1 - locationList[i];
    }

    return pResult;
}

// 
// -----------------------------------------------------------------------------
