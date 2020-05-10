/*******************************************************************************
 *  @file      ReedSolomonEncode.h 2013\8\19 16:06:58 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com>(中文编码测试)
 
 ******************************************************************************/

#ifndef REEDSOLOMONENCODE_161E611B_EC82_48F1_8EEE_CE74F5125915_H__
#define REEDSOLOMONENCODE_161E611B_EC82_48F1_8EEE_CE74F5125915_H__

#include "GaloisFieldPolynomial.h"
/******************************************************************************/

/**
 * The class <code>ReedSolomonEncode</code>
 *
 */
class ReedSolomonEncode
{
public:
    /** @name Constructors and Destructor*/

    //@{
    /**
     * 构造RS编码对象
     *
     * @param   const std::size_t datalength               数据区长度 单位 ： 字节
     * @param   const std::size_t checklenth               校验区长度 单位 ： 字节
     * @param   const GaloisField & gfield                 引用的伽罗华域
     * @param   const GaloisFieldPolynomial & generatePoly 引用生成多项式
     */
    ReedSolomonEncode(const std::size_t datalength,
                      const std::size_t checklenth,
                      const GaloisField& gfield,
                      const GaloisFieldPolynomial& generatePoly);
    /**
     * Destructor
     */
    ~ReedSolomonEncode();
    //@}

public:
    /** @name Setter Methods*/
    //@{
    /**
     * RS编码
     *
     * @param   const unsigned char * data       要编码的数据区，要求长度为 datalength
     * @param   unsigned char *       fecData    要编码的校验区，要求长度为 checklenth
     * @return  bool                             正确编码返回true， fecData指向的长度 checklenth 的区域
     *                                           被计算的校验码填满
     */
    bool encode(const unsigned char* data, unsigned char* fecData) const;
    //@}
public:
    /** @name Getter Methods*/
    //@{

    //@}
public:
    /** @name Access Methods*/
    //@{

    //@}   
protected:

private:

    const std::size_t             m_code_length;
    const std::size_t             m_data_length;
    const std::size_t             m_fec_length;
    const GaloisField&            m_gfield;
    const GaloisFieldPolynomial&  m_generatorPoly;

};


/******************************************************************************/
#endif// REEDSOLOMONENCODE_161E611B_EC82_48F1_8EEE_CE74F5125915_H__
