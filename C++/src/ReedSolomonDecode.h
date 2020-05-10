/*******************************************************************************
 *  @file      ReedSolomonDecode.h 2013\8\20 10:01:27 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com>(中文编码测试)
 
 ******************************************************************************/

#ifndef REEDSOLOMONDECODE_26BE2AF1_0195_42B1_9B4E_7DEE1EF43B6F_H__
#define REEDSOLOMONDECODE_26BE2AF1_0195_42B1_9B4E_7DEE1EF43B6F_H__

#include "GaloisFieldPolynomial.h"
/******************************************************************************/

/**
 * The class <code>ReedSolomonDecode</code> RS代码解码
 *
 */
class ReedSolomonDecode
{
public:
    /** @name Constructors and Destructor*/

    //@{
    /**
     * No Parameter Constructor 
     */
    ReedSolomonDecode( const std::size_t datalength,
                       const std::size_t checklenth,
                       const GaloisField& gfield,
                       const unsigned int genInitialIndex
                     );
    /**
     * Destructor
     */
    ~ReedSolomonDecode();
    //@}

public:
    /** @name Setter Methods*/
    //@{

    //@}
public:
    /** @name Getter Methods*/
    //@{

    //@}
public:
    /** @name Access Methods*/
    //@{
    /**
     * 检测RS编码的正确性， 
     *
     * @param   const unsigned char * pData   RS编码的数据区域， 长度为 datalength + checklenth
     * @return  bool    编码有错误返回   False
     *                  编码没有错误返回  True
     *                  当错误数量超过 checklenth 个的时候返回值不确定（无法有效检测超过checklenth的错误）
     */
    bool checkData(const unsigned char* pData) const;

    /**
     * 解码RS编码，即：RS纠错， (本函数除了纠偏数据区域错误位置列表指明的错误外，
     *                        还可纠偏 （checklenth - locationLength）/2 个未知位置错误)
     *
     * @param   unsigned char *    pData     RS编码的数据区域， 长度为 datalength + checklenth
     * @param   const int * locationList     数据区域错误位置列表
     * @param   const int locationLength     数据区域错误位置列表长度
     * @return  bool                         true 正确纠偏
     *                                       false 纠偏错误
     */
    bool decode( unsigned char* pData, 
                 const int* locationList, 
                 const int locationLength) const;
    //@}   
protected:
    void init();


    int computeSyndrome(const GaloisFieldPolynomial& received,
                              GaloisFieldPolynomial& syndrome) const;
    void loadData(GaloisFieldPolynomial& received, 
                    const unsigned char* pData)                const;

    void computeGamma( GaloisFieldPolynomial& gamma, 
                        const int* erasure_locations, 
                        const int locationLenth )              const;

    void computeDiscrepancy(GaloisFieldElement&          discrepancy,
                             const GaloisFieldPolynomial&  lambda,
                             const GaloisFieldPolynomial&  syndrome,
                             const std::size_t&            l,
                             const std::size_t&            round) const;

    void modifiedBerlekampMasseyAlgorithm(GaloisFieldPolynomial&       lambda,
                                          const GaloisFieldPolynomial& syndrome,
                                          const std::size_t               erasure_count) const;

    int ChienSearchRoots(const GaloisFieldPolynomial& poly, int* root_list) const;

    bool forneyAlgorithm(const int *    error_locations,
                          const int      locationlength,
                          const GaloisFieldPolynomial&  lambda,
                          const GaloisFieldPolynomial&  syndrome,
                          unsigned char*                   pRsblock)  const;

private:
    int* changeLocation(const int* locationList, const int locationLength) const;
private:

    const std::size_t                   m_code_length;
    const std::size_t                   m_data_length;
    const std::size_t                   m_fec_length;
    const GaloisField&                  m_gfield;
    std::vector<field_symbol>           m_rootExponentTable;
    std::vector<field_symbol>           m_syndromeExponentTable;
    std::vector<GaloisFieldPolynomial>  m_gammaTable;
    const GaloisFieldPolynomial         m_XValue;
    unsigned int                        m_genInitialIndex;
};

/******************************************************************************/
#endif// REEDSOLOMONDECODE_26BE2AF1_0195_42B1_9B4E_7DEE1EF43B6F_H__
