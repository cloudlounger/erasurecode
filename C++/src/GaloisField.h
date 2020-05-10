
/*******************************************************************************
 *  @file      GaloisField.h 2013\8\8 9:25:24 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com>(中文编码测试)
 ******************************************************************************/

#ifndef GALOISFIELD_A998B270_59CC_48C2_AE3C_13B2CFA45D3B_H__
#define GALOISFIELD_A998B270_59CC_48C2_AE3C_13B2CFA45D3B_H__

#include <iostream>
/******************************************************************************/


/** @name primitive polynomial 本原多项式 */
//{

/*1x^0 + 1x^1 + 1x^2*/ 
    /* x^2 + x + 1 */
const unsigned int primitive_polynomial_power_2[]     = {1, 1 , 1};
const unsigned int primitive_polynomial_power_2_size  = 3;
/* 1x^0 + 1x^1 + 0x^2 + 1x^3 */ 
    /* x^3 + x + 1 */
const unsigned int primitive_polynomial_power_3[]     = {1, 1, 0, 1};
const unsigned int primitive_polynomial_power_3_size  = 4;
/* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 1x^4*/ 
    /* x^4 + x + 1 */
const unsigned int primitive_polynomial_power_4[]     = {1, 1, 0, 0, 1};
const unsigned int primitive_polynomial_power_4_size  = 5;
/* 1x^0 + 0x^1 + 1x^2 + 0x^3 + 0x^4 + 1x^5 */ 
    /* x^5 + x^2 + 1 */ 
const unsigned int primitive_polynomial_power_5[]     = {1, 0, 1, 0, 0, 1};
const unsigned int primitive_polynomial_power_5_size  = 6;
/* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 0x^4 + 0x^5 + 1x^6 */ 
    /* x^6 + x + 1 */
const unsigned int primitive_polynomial_power_6[]     = {1, 1, 0, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_6_size  = 7;
/* 1x^0 + 0x^1 + 0x^2 + 1x^3 + 0x^4 + 0x^5 + 0x^6 + 1x^7 */ 
    /* x^7 + x^3 + 1 */
const unsigned int primitive_polynomial_power_7[]     = {1, 0, 0, 1, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_7_size  = 8;

/* 1x^0 + 0x^1 + 1x^2 + 1x^3 + 1x^4 + 0x^5 + 0x^6 + 0x^7 + 1x^8 */ 
    /* x^8 + x^4 + x^3 + x^2 + 1 */
const unsigned int primitive_polynomial_power_8_0[]   = {1, 0, 1, 1, 1, 0, 0, 0, 1};
/* 1x^0 + 1x^1 + 1x^2 + 0x^3 + 0x^4 + 0x^5 + 0x^6 + 1x^7 + 1x^8 */ 
    /* x^8 + x^7 + x^2 + x + 1 */
const unsigned int primitive_polynomial_power_8_1[]   = {1, 1, 1, 0, 0, 0, 0, 1, 1};
const unsigned int primitive_polynomial_power_8_size  = 9;

/* 1x^0 + 0x^1 + 0x^2 + 0x^3 + 1x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 1x^9 */
    /* x^9 + x^4 + 1 */
const unsigned int primitive_polynomial_power_9[]     = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_9_size  = 10;

/* 1x^0 + 0x^1 + 0x^2 + 1x^3 + 0x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 1x^10 */
    /* x^10 + x^3 + 1 */
const unsigned int primitive_polynomial_power_10[]    = {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_10_size = 11;

/* 1x^0 + 0x^1 + 1x^2 + 0x^3 + 0x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 1x^11 */
    /* x^11 + x^2 + 1 */
const unsigned int primitive_polynomial_power_11[]    = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_11_size = 12;

/* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 1x^4 + 0x^5 + 1x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 0x^11 + 1x^12 */
    /* x^12 + x^6 + x^4 + x + 1 */
const unsigned int primitive_polynomial_power_12[]    = {1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_12_size = 13;

/* 1x^0 + 1x^1 + 0x^2 + 1x^3 + 1x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 0x^11 + 0x^12 + 1x^13 */
    /* x^13 + x^4 + x^3 + x + 1 */
const unsigned int primitive_polynomial_power_13[]    = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_13_size = 14;

/* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 0x^4 + 0x^5 + 1x^6 + 0x^7 + 0x^8 + 0x^9 + 1x^10 + 0x^11 + 0x^12 + 0x^13 + 1x^14 */
    /* x^14 + x^10 + x^6 + x + 1 */
const unsigned int primitive_polynomial_power_14[]    = {1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_14_size = 15;

/* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 0x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 0x^11 + 0x^12 + 0x^13 + 0x^14 + 1x^15 */
    /* x^15 + x + 1 */
const unsigned int primitive_polynomial_power_15[]    = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_15_size = 16;

/* 1x^0 + 1x^1 + 0x^2 + 1x^3 + 0x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 0x^11 + 1x^12 + 0x^13 + 0x^14 + 0x^15 + 1x^16 */
    /* x^16 + x^12 + x^3 + x + 1 */
const unsigned int primitive_polynomial_power_16[]    = {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1};
const unsigned int primitive_polynomial_power_16_size = 17;


//@}


typedef int          field_symbol;
const   field_symbol GFERROR = -1;

/**
 * The class <code>GaloisField</code>伽罗华域
 */
class GaloisField
{
public:
    /** @name Constructors and Destructor*/

    GaloisField( const unsigned int pwr, const unsigned int* primitive_poly, const unsigned int primpoly_deg);
    /**
     * Destructor
     */
    ~GaloisField();
    //@}

public:
    bool operator==(const GaloisField& gf) const;
    bool operator!=(const GaloisField& gf) const;


private:
    GaloisField();
public:
    /** @name Getter Methods*/
    //@{
    field_symbol index(const field_symbol value) const;
    field_symbol alpha(const field_symbol value) const;
    unsigned int size()             const;
    unsigned int pwr()              const;
    unsigned int mask()             const;

    //@}
public:
    /** @name 运算符方 */
    //@{

    /**
     * 加法 运算 
     */
    field_symbol add       (const field_symbol& a, const field_symbol& b) const;
    /**
     * 减法 运算 
     */
    field_symbol sub       (const field_symbol& a, const field_symbol& b) const;
    /**
     * 乘法 运算 
     */
    field_symbol mul       (const field_symbol& a, const field_symbol& b) const;
    /**
     * 除法 运算 
     */
    field_symbol div       (const field_symbol& a, const field_symbol& b) const;
    /**
     * 幂指数 运算 
     */
    field_symbol exp       (const field_symbol& a, const std::size_t&  n) const;
    /**
     * 求一个元素的乘法逆元素
     */
    field_symbol inverse   (const field_symbol& val)                      const;

    /**
     * 转换元素的同余划分到标准基值
     */
    field_symbol normalize (field_symbol x)                               const;
    //@}   

private:
    void  generate_field();
    field_symbol gen_mul       (const field_symbol& a, const field_symbol& b) const;
    field_symbol gen_div       (const field_symbol& a, const field_symbol& b) const;
    field_symbol gen_exp       (const field_symbol& a, const std::size_t&  n) const;
    field_symbol gen_inverse   (const field_symbol& val)                      const;

private:
    std::size_t create_array(char buffer_[],
                             const std::size_t& length,
                             const std::size_t offset,
                             field_symbol** array);

   std::size_t create_2d_array(char buffer_[], 
                               std::size_t row_cnt, 
                               std::size_t col_cnt,
                               const std::size_t offset,
                               field_symbol*** array);
private:
    unsigned int         power_;             ///< 指数   
    std::size_t          prim_poly_deg_;     ///< 多项式高最次幂   
    unsigned int         field_size_;        ///< 域的元素个数 2^power_ - -1 个    
    unsigned int         prim_poly_hash_;    ///< hash 表，用于对象比较实用    
    const unsigned int*  prim_poly_;         ///< 引用本原多项式   

    field_symbol*  alpha_to_;                ///< key->vaule
    field_symbol*  index_of_;                ///< value->key

#if !defined(DYNAMIC_COMPUTER)
    field_symbol*  mul_inverse_;            ///< 乘法逆元表   
    field_symbol** mul_table_;              ///< 乘法表   
    field_symbol** div_table_;              ///< 除法表   
    field_symbol** exp_table_;              ///< 幂运算表   
    char*          buffer_;                 ///< 辅助空间，分配了以上4个空间   

#endif //DYNAMIC_COMPUTER
};

std::ostream& operator << (std::ostream& os, const GaloisField& gf);

/******************************************************************************/
#endif// GALOISFIELD_A998B270_59CC_48C2_AE3C_13B2CFA45D3B_H__

