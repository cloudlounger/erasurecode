/*******************************************************************************
 *  @file      GaloisFieldPolynomial.h 2013\8\9 14:23:34 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com>(中文编码测试)
 
 ******************************************************************************/

#ifndef GALOISFIELDPOLYNOMIAL_DDA139D9_9B2C_4534_8163_CFE3B242614E_H__
#define GALOISFIELDPOLYNOMIAL_DDA139D9_9B2C_4534_8163_CFE3B242614E_H__

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

#include "GaloisField.h"
#include "GaloisFieldElement.h"
/******************************************************************************/

/**
 * The class <code>GaloisFieldPolynomial</code>基于伽罗华域的多项式
 * 
 */
class GaloisFieldPolynomial
{
public:

    GaloisFieldPolynomial(const GaloisField& gfield);
    GaloisFieldPolynomial(const GaloisField& gfield, const unsigned int& degree);
    GaloisFieldPolynomial(const GaloisField& gfield, 
                          const unsigned int& degree, 
                          GaloisFieldElement element[]);
    /**
     * 拷贝构造函数
     */
    GaloisFieldPolynomial(const GaloisFieldPolynomial& polynomial);
    GaloisFieldPolynomial(const GaloisFieldElement& gfe);
    ~GaloisFieldPolynomial();;

    bool valid()                            const;
    int deg()                               const;
    const GaloisField& galois_field()       const;
    void set_degree(const unsigned int& x);
  
    /**
     * 简化多项式（注意现在有bug，先不要使用(如果是0元，简化过于暴力)）
     *
     * @return  void
     */
    void simplify();

    GaloisFieldPolynomial& operator   =  (const GaloisFieldPolynomial& polynomial);
    GaloisFieldPolynomial& operator   =  (const GaloisFieldElement&       element);
    GaloisFieldPolynomial& operator  +=  (const GaloisFieldPolynomial&    element);
    GaloisFieldPolynomial& operator  +=  (const GaloisFieldElement&       element);
    GaloisFieldPolynomial& operator  -=  (const GaloisFieldPolynomial&    element);
    GaloisFieldPolynomial& operator  -=  (const GaloisFieldElement&       element);
    GaloisFieldPolynomial& operator  *=  (const GaloisFieldPolynomial& polynomial);
    GaloisFieldPolynomial& operator  *=  (const GaloisFieldElement&       element);
    GaloisFieldPolynomial& operator  /=  (const GaloisFieldPolynomial&    divisor);
    GaloisFieldPolynomial& operator  /=  (const GaloisFieldElement&       element);
    GaloisFieldPolynomial& operator  %=  (const GaloisFieldPolynomial&    divisor);
    GaloisFieldPolynomial& operator  %=  (const unsigned int&             power);
    GaloisFieldPolynomial& operator  ^=  (const unsigned int&                 n);
    GaloisFieldPolynomial& operator <<=  (const unsigned int&                 n);
    GaloisFieldPolynomial& operator >>=  (const unsigned int&                 n);

    GaloisFieldElement&    operator[] (const unsigned int&              term);
    GaloisFieldElement     operator() (const GaloisFieldElement&       value);
    GaloisFieldElement     operator() (field_symbol                    value);

    const GaloisFieldElement&  operator[](const unsigned int&   term)      const;
    const GaloisFieldElement   operator()(const GaloisFieldElement& value) const;
    const GaloisFieldElement   operator()(field_symbol         value)      const;

    bool operator==(const GaloisFieldPolynomial& polynomial) const;
    bool operator!=(const GaloisFieldPolynomial& polynomial) const;

    //bool monic() const;

    /**
     * 多项式求导数
     *
     * @return  GaloisFieldPolynomial
     * @exception 
     */
    GaloisFieldPolynomial derivative() const;

    std::string str() const;

    std::string simplify_str() const;

    friend std::ostream& operator << (std::ostream& os, const GaloisFieldPolynomial& polynomial);

private:

    typedef std::vector< GaloisFieldElement >::iterator       PolyIterator;
    typedef std::vector< GaloisFieldElement >::const_iterator ConstPolyIterator;

    //void simplify(GaloisFieldPolynomial& polynomial) const;

    GaloisField& m_gfield;
    std::vector<GaloisFieldElement> m_poly;   
};


GaloisFieldPolynomial operator + (const GaloisFieldPolynomial&   a, const GaloisFieldPolynomial&   b);
GaloisFieldPolynomial operator + (const GaloisFieldPolynomial&   a, const GaloisFieldElement&      b);
GaloisFieldPolynomial operator + (const GaloisFieldElement&      a, const GaloisFieldPolynomial&   b);
GaloisFieldPolynomial operator + (const GaloisFieldPolynomial&   a, const field_symbol&            b);
GaloisFieldPolynomial operator + (const field_symbol&       a, const GaloisFieldPolynomial&        b);

GaloisFieldPolynomial operator - (const GaloisFieldPolynomial&   a, const GaloisFieldPolynomial&   b);
GaloisFieldPolynomial operator - (const GaloisFieldPolynomial&   a, const GaloisFieldElement&      b);
GaloisFieldPolynomial operator - (const GaloisFieldElement& a, const GaloisFieldPolynomial&        b);
GaloisFieldPolynomial operator - (const GaloisFieldPolynomial&   a, const field_symbol&            b);
GaloisFieldPolynomial operator - (const field_symbol&       a, const GaloisFieldPolynomial&        b);

GaloisFieldPolynomial operator * (const GaloisFieldPolynomial&   a, const GaloisFieldPolynomial&   b);
GaloisFieldPolynomial operator * (const GaloisFieldElement&      a, const GaloisFieldPolynomial&   b);
GaloisFieldPolynomial operator * (const GaloisFieldPolynomial&   a, const GaloisFieldElement&      b);

GaloisFieldPolynomial operator / (const GaloisFieldPolynomial&   a, const GaloisFieldPolynomial&   b);
GaloisFieldPolynomial operator / (const GaloisFieldPolynomial&   a, const GaloisFieldElement&      b);

GaloisFieldPolynomial operator % (const GaloisFieldPolynomial&   a, const GaloisFieldPolynomial&   b);
GaloisFieldPolynomial operator % (const GaloisFieldPolynomial&   a, const unsigned int&        power);
GaloisFieldPolynomial operator ^ (const GaloisFieldPolynomial&   a, const int&                n);
GaloisFieldPolynomial operator <<(const GaloisFieldPolynomial&   a, const unsigned int&       n);
GaloisFieldPolynomial operator >>(const GaloisFieldPolynomial&   a, const unsigned int&       n);
//GaloisFieldPolynomial        gcd (const GaloisFieldPolynomial&   a, const GaloisFieldPolynomial&   b);
std::ostream&         operator << (std::ostream& os, const GaloisFieldPolynomial& polynomial);

GaloisFieldPolynomial generateXValue (const GaloisField& gfield);

bool generatorPolynomialCreator(const GaloisField& field,
                                const std::size_t initial_index,
                                const std::size_t num_elements,
                                GaloisFieldPolynomial& generator_polynomial);

/******************************************************************************/
#endif// GALOISFIELDPOLYNOMIAL_DDA139D9_9B2C_4534_8163_CFE3B242614E_H__
