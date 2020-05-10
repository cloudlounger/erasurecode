/*******************************************************************************
 *  @file      GaloisFieldElement.h 2013\8\9 11:40:37 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com>(中文编码测试)
 
 ******************************************************************************/

#ifndef GALOISFIELDELEMENT_4CB32562_1A57_42E0_A671_38364553137B_H__
#define GALOISFIELDELEMENT_4CB32562_1A57_42E0_A671_38364553137B_H__

#include "GaloisField.h"
/******************************************************************************/

/**
 * The class <code>GaloisFieldElement</code>
 */
class GaloisFieldElement
    {
public:
    /** @name Constructors and Destructor*/

    //@{
    GaloisFieldElement(const GaloisField& gfield);
    GaloisFieldElement(const GaloisField& gfield,const field_symbol& v);

    GaloisFieldElement(const GaloisFieldElement& gele);

    GaloisFieldElement& operator = (const GaloisFieldElement& gfe);
    GaloisFieldElement& operator = (const field_symbol& v);
    /**
     * Destructor
     */
    ~GaloisFieldElement();
    //@}

public:
    /** @name Operator Methods*/
    //@{
    GaloisFieldElement& operator += (const GaloisFieldElement& gfe);
    GaloisFieldElement& operator += (const field_symbol& v);

    GaloisFieldElement& operator -= (const GaloisFieldElement& gfe);
    GaloisFieldElement& operator -= (const field_symbol& v);

    GaloisFieldElement& operator *= (const GaloisFieldElement& gfe);
    GaloisFieldElement& operator *= (const field_symbol& v);

    GaloisFieldElement& operator /= (const GaloisFieldElement& gfe);
    GaloisFieldElement& operator /= (const field_symbol& v);

    GaloisFieldElement& operator ^= (const int& n);

    bool operator == (const GaloisFieldElement& gfe) const;
    bool operator == (const field_symbol& v) const;

    bool operator != (const GaloisFieldElement& gfe) const;
    bool operator != (const field_symbol& v) const;

    bool operator < (const GaloisFieldElement& gfe);
    bool operator < (const field_symbol& v);

    bool operator > (const GaloisFieldElement& gfe);
    bool operator > (const field_symbol& v);

    field_symbol index() const;

    field_symbol poly() const;

    const GaloisField& galois_field() const;

    field_symbol inverse() const;

    void normalize();
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
private:

    const GaloisField&  field_;
    field_symbol        poly_value_;

};

GaloisFieldElement operator + (const GaloisFieldElement& a, const GaloisFieldElement& b);
GaloisFieldElement operator - (const GaloisFieldElement& a, const GaloisFieldElement& b);
GaloisFieldElement operator * (const GaloisFieldElement& a, const GaloisFieldElement& b);
GaloisFieldElement operator * (const GaloisFieldElement& a, const field_symbol&       b);
GaloisFieldElement operator * (const field_symbol&       a, const GaloisFieldElement& b);
GaloisFieldElement operator / (const GaloisFieldElement& a, const GaloisFieldElement& b);
GaloisFieldElement operator ^ (const GaloisFieldElement& a, const int&                b);      

/******************************************************************************/
#endif// GALOISFIELDELEMENT_4CB32562_1A57_42E0_A671_38364553137B_H__
