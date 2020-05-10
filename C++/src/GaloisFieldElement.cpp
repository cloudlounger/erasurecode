/******************************************************************************* 
 *  @file      GaloisFieldElement.cpp 2013\8\9 13:28:49 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 ******************************************************************************/

#include "GaloisFieldElement.h"

/******************************************************************************/

GaloisFieldElement::GaloisFieldElement( const GaloisField& gfield )
    : field_(gfield)
    , poly_value_(-1)
{}

GaloisFieldElement::GaloisFieldElement( const GaloisField& gfield,const field_symbol& v)
    : field_(gfield)
    , poly_value_(v)
{}

 GaloisFieldElement::GaloisFieldElement( const GaloisFieldElement& gele )
    : field_(gele.field_)
    , poly_value_(gele.poly_value_)
{}

GaloisFieldElement& GaloisFieldElement::operator=( const GaloisFieldElement& gfe )
{
    if (this == &gfe) return *this;
    const_cast<GaloisField&>(field_) = gfe.field_;
    poly_value_  = gfe.poly_value_;
    return *this;
}

GaloisFieldElement& GaloisFieldElement::operator=( const field_symbol& v )
{
    poly_value_  = (v & field_.size());
    return *this;
}

GaloisFieldElement::~GaloisFieldElement()
{}


GaloisFieldElement& GaloisFieldElement::operator += (const GaloisFieldElement& gfe)
{
    poly_value_ ^= gfe.poly_value_;
    return *this;
}

GaloisFieldElement& GaloisFieldElement::operator += (const field_symbol& v)
{
    poly_value_ ^= v;
    return *this;
}

GaloisFieldElement& GaloisFieldElement::operator -= (const GaloisFieldElement& gfe)
{
    *this += gfe;
    return *this;
}

GaloisFieldElement& GaloisFieldElement::operator -= (const field_symbol& v)
{
    *this += v;
    return *this;
}

GaloisFieldElement& GaloisFieldElement::operator *= (const GaloisFieldElement& gfe)
{
    poly_value_ = field_.mul(poly_value_, gfe.poly_value_);
    return *this;
}

GaloisFieldElement& GaloisFieldElement::operator *= (const field_symbol& v)
{
    poly_value_ = field_.mul(poly_value_, v);
    return *this;
}


 GaloisFieldElement& GaloisFieldElement::operator /= ( const GaloisFieldElement& gfe )
{
    poly_value_ = field_.div(poly_value_,gfe.poly_value_);
    return *this;
}

GaloisFieldElement& GaloisFieldElement::operator /= ( const field_symbol& v )
{
    poly_value_ = field_.div(poly_value_,v);
    return *this;
}

GaloisFieldElement& GaloisFieldElement::operator ^= ( const int& n )
{
    poly_value_ = field_.exp(poly_value_,n);
    return *this;
}

bool GaloisFieldElement::operator == ( const GaloisFieldElement& gfe ) const
{
    return ((field_  == gfe.field_) && (poly_value_ == gfe.poly_value_));
}

 bool GaloisFieldElement::operator == ( const field_symbol& v ) const
{
    return (poly_value_ == v);
}


bool GaloisFieldElement::operator != ( const GaloisFieldElement& gfe ) const
{
    return ((field_ != gfe.field_) || (poly_value_ != gfe.poly_value_));
}

bool GaloisFieldElement::operator != ( const field_symbol& v ) const
{
    return (poly_value_ != v);
}

bool GaloisFieldElement::operator < ( const GaloisFieldElement& gfe )
{
    return (poly_value_ < gfe.poly_value_);
}

bool GaloisFieldElement::operator < ( const field_symbol& v )
{
    return (poly_value_ < v);
}

bool GaloisFieldElement::operator > ( const GaloisFieldElement& gfe )
{
    return (poly_value_ > gfe.poly_value_);
}

bool GaloisFieldElement::operator > ( const field_symbol& v )
{
    return (poly_value_ > v);
}


field_symbol GaloisFieldElement::index() const
{
    return field_.index(poly_value_);
}


field_symbol GaloisFieldElement::poly() const
{
    return poly_value_;
}


const GaloisField& GaloisFieldElement::galois_field() const
{
    return field_;
}

field_symbol GaloisFieldElement::inverse() const
{
    return field_.inverse(poly_value_);
}

void GaloisFieldElement::normalize()
{
    poly_value_ &= field_.size();
}


//

GaloisFieldElement operator + (const GaloisFieldElement& a, const GaloisFieldElement& b)
{
    GaloisFieldElement result = a;
    result += b;
    return result;
}

GaloisFieldElement operator - (const GaloisFieldElement& a, const GaloisFieldElement& b)
{
    GaloisFieldElement result = a;
    result -= b;
    return result;
}

GaloisFieldElement operator * (const GaloisFieldElement& a, const GaloisFieldElement& b)
{
    GaloisFieldElement result = a;
    result *= b;
    return result;
}

GaloisFieldElement operator * (const GaloisFieldElement& a, const field_symbol& b)
{
    GaloisFieldElement result = a;
    result *= b;
    return result;
}

GaloisFieldElement operator * (const field_symbol& a, const GaloisFieldElement& b)
{
    GaloisFieldElement result = b;
    result *= a;
    return result;
}

GaloisFieldElement operator / (const GaloisFieldElement& a, const GaloisFieldElement& b)
{
    GaloisFieldElement result = a;
    result /= b;
    return result;
}

GaloisFieldElement operator ^ (const GaloisFieldElement& a, const int& b)
{
    GaloisFieldElement result = a;
    result ^= b;
    return result;
}