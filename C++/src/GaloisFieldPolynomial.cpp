/******************************************************************************* 
 *  @file      GaloisFieldPolynomial.cpp 2013\8\9 14:28:58 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 ******************************************************************************/

#include "GaloisFieldPolynomial.h"
#include <sstream>
/******************************************************************************/

// -----------------------------------------------------------------------------
// public   
GaloisFieldPolynomial::GaloisFieldPolynomial(const GaloisField& gfield)
    : m_gfield(const_cast<GaloisField&>(gfield))
{
    m_poly.clear();
    m_poly.reserve(256);
}

// -----------------------------------------------------------------------------
// public   
GaloisFieldPolynomial::GaloisFieldPolynomial( const GaloisField& gfield, 
    const unsigned int& degree)
    : m_gfield(const_cast<GaloisField&>(gfield))
{
    m_poly.reserve(256);
    m_poly.resize(degree + 1,GaloisFieldElement(m_gfield,0));
}




// -----------------------------------------------------------------------------
// public   
GaloisFieldPolynomial::GaloisFieldPolynomial( const GaloisField& gfield, 
                                              const unsigned int& degree, 
                                              GaloisFieldElement element[] ) 
    : m_gfield(const_cast<GaloisField&>(gfield))
{
    m_poly.reserve(256);
    if (element != NULL)
    {
        /*
        It is assumed that element is an array of field elements
        with size/element count of degree + 1.
        */
        for (unsigned int i = 0; i <= degree; ++i)
        {
            m_poly.push_back(element[i]);
        }
    }
    else
    {
        m_poly.resize(degree + 1,GaloisFieldElement(m_gfield,0));
    }
}

GaloisFieldPolynomial::GaloisFieldPolynomial(const GaloisFieldPolynomial& polynomial)
    : m_gfield(const_cast<GaloisField&>(polynomial.m_gfield)),
    m_poly(polynomial.m_poly)
{}

GaloisFieldPolynomial::GaloisFieldPolynomial(const GaloisFieldElement& element)

    : m_gfield(const_cast<GaloisField&>(element.galois_field()))
{
    m_poly.resize(1,element);
}


// -----------------------------------------------------------------------------
// public   
GaloisFieldPolynomial::~GaloisFieldPolynomial()
{}

bool GaloisFieldPolynomial::valid() const
{
    return (m_poly.size() > 0);
}

int GaloisFieldPolynomial::deg() const
{
    return static_cast<int>(m_poly.size()) - 1;
}

const GaloisField& GaloisFieldPolynomial::galois_field() const
{
    return m_gfield;
}

void GaloisFieldPolynomial::set_degree(const unsigned int& x)
{
    m_poly.resize(x + 1,GaloisFieldElement(m_gfield,0));
}

void GaloisFieldPolynomial::simplify()
{
    std::size_t poly_size = this->m_poly.size();
    if ((poly_size > 0) && (this->m_poly.back() == 0))
    {
        PolyIterator it = this->m_poly.end();
        std::size_t count = 0;
        while (this->m_poly.begin() != it)
        {
            if ((*(--it)) == 0)
                ++count;
            else
                break;
        }
        if (0 != count)
        {
            if (count == poly_size)
            {
                this->m_poly.resize(poly_size - count + 1, GaloisFieldElement(m_gfield,0));
            }
            else
            {
                this->m_poly.resize(poly_size - count, GaloisFieldElement(m_gfield,0));
            }

        }
    }
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator=(const GaloisFieldPolynomial& polynomial)
{
    if (this == &polynomial)
    {
        return *this;
    }
    m_gfield = polynomial.m_gfield;
    m_poly  = polynomial.m_poly;
    return *this;
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator=(const GaloisFieldElement& element)
{
    m_gfield = element.galois_field();
    m_poly.resize(1,element);
    return *this;
}



GaloisFieldPolynomial& GaloisFieldPolynomial::operator += (const GaloisFieldPolynomial& polynomial)
{
    if (m_gfield == polynomial.m_gfield)
    {
        if (m_poly.size() < polynomial.m_poly.size())
        {
            ConstPolyIterator it0 = polynomial.m_poly.begin();
            for (PolyIterator it1 = m_poly.begin(); it1 != m_poly.end(); ++it0, ++it1)
            {
                (*it1) += (*it0);
            }

            while (it0 != polynomial.m_poly.end())
            {
                m_poly.push_back(*it0);
                ++it0;
            }
        }
        else
        {
            PolyIterator it0 = m_poly.begin();
            for (ConstPolyIterator it1 = polynomial.m_poly.begin(); it1 != polynomial.m_poly.end(); ++it0, ++it1)
            {
                (*it0) += (*it1);
            }
        }

        //simplify(*this);
    }

    return *this;
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator+=(const GaloisFieldElement& element)
{
    m_poly[0] += element;
    return *this;
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator-=(const GaloisFieldPolynomial& element)
{
    return (*this += element);
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator-=(const GaloisFieldElement& element)
{
    m_poly[0] -= element;
    return *this;
}


GaloisFieldPolynomial& GaloisFieldPolynomial::operator *= (const GaloisFieldPolynomial& polynomial)
{
    if (m_gfield == polynomial.m_gfield)
    {
        GaloisFieldPolynomial product(m_gfield,deg() + polynomial.deg() + 1);

        PolyIterator result_it = product.m_poly.begin();
        for (PolyIterator it0 = m_poly.begin(); it0 != m_poly.end(); ++it0)
        {
            PolyIterator current_result_it = result_it;
            for (ConstPolyIterator it1 = polynomial.m_poly.begin(); it1 != polynomial.m_poly.end(); ++it1)
            {
                (*current_result_it) += (*it0) * (*it1);
                ++current_result_it;
            }
            ++result_it;
        }

        //simplify(product);

        m_poly = product.m_poly;
    }
    return *this;
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator*=(const GaloisFieldElement& element)
{
    if (m_gfield == element.galois_field())
    {
        for (PolyIterator it = m_poly.begin(); it != m_poly.end(); ++it)
        {
            (*it) *= element;
        }
    }
    return *this;
}


GaloisFieldPolynomial& GaloisFieldPolynomial::operator/=(const GaloisFieldPolynomial& divisor)
{
    if (
        (m_gfield        == divisor.m_gfield) &&
        (deg()         >= divisor.deg())  &&
        (divisor.deg() >=             0)
        )
    {
        GaloisFieldPolynomial quotient(m_gfield, deg() - divisor.deg() + 1);
        GaloisFieldPolynomial remainder(m_gfield, divisor.deg() - 1);

        for (int i = static_cast<int>(deg()); i >= 0; i--)
        {
            if (i <= static_cast<int>(quotient.deg()))
            {
                quotient[i] = remainder[remainder.deg()] / divisor[divisor.deg()];

                for (int j = static_cast<int>(remainder.deg()); j > 0; --j)
                {
                    remainder[j] = remainder[j - 1] + (quotient[i] * divisor[j]);
                }

                remainder[0] = m_poly[i] + (quotient[i] * divisor[0]);
            }
            else
            {
                for (int j = static_cast<int>(remainder.deg()); j > 0; --j)
                {
                    remainder[j] = remainder[j - 1];
                }
                remainder[0] = m_poly[i];
            }
        }

        //simplify(quotient);
        
        m_poly = quotient.m_poly;
    }

    return *this;
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator/=(const GaloisFieldElement& element)
{
    if (m_gfield == element.galois_field())
    {
        for (PolyIterator it = m_poly.begin(); it != m_poly.end(); ++it)
        {
            (*it) /= element;
        }
    }
    return *this;
}



GaloisFieldPolynomial& GaloisFieldPolynomial::operator%=(const GaloisFieldPolynomial& divisor)
{
    if (
        (m_gfield        == divisor.m_gfield) &&
        (deg()         >= divisor.deg())  &&
        (divisor.deg() >=             0)
        )
    {
        GaloisFieldPolynomial quotient (m_gfield, deg() - divisor.deg() + 1);
        GaloisFieldPolynomial remainder(m_gfield, divisor.deg() - 1);

        for (int i = static_cast<int>(deg()); i >= 0; i--)
        {
            if (i <= static_cast<int>(quotient.deg()))
            {
                quotient[i] = remainder[remainder.deg()] / divisor[divisor.deg()];

                for (int j = static_cast<int>(remainder.deg()); j > 0; --j)
                {
                    remainder[j] = remainder[j - 1] + (quotient[i] * divisor[j]);
                }
                remainder[0] = m_poly[i] + (quotient[i] * divisor[0]);
            }
            else
            {
                for (int j = static_cast<int>(remainder.deg()); j > 0; --j)
                {
                    remainder[j] = remainder[j - 1];
                }
                remainder[0] = m_poly[i];
            }
        }
        m_poly = remainder.m_poly;
    }
    return *this;
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator%=(const unsigned int& power)
{
    if (m_poly.size() >= power)
    {
        m_poly.resize(power,GaloisFieldElement(m_gfield,0));
        //simplify(*this);
    }
    return *this;
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator^=(const unsigned int& n)
{
    GaloisFieldPolynomial result = *this;

    for (std::size_t i = 0; i < n; ++i)
    {
        result *= *this;
    }

    *this = result;

    return *this;
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator<<=(const unsigned int& n)
{
    if (m_poly.size() > 0)
    {
        size_t initial_size = m_poly.size();
        m_poly.resize(m_poly.size() + n, GaloisFieldElement(m_gfield,0));

        for (size_t i = initial_size - 1; static_cast<int>(i) >= 0; --i)
        {
            m_poly[i + n] = m_poly[i];
        }

        for (unsigned int i = 0; i < n; ++i)
        {
            m_poly[i] = 0;
        }
    }
    return *this;
}

GaloisFieldPolynomial& GaloisFieldPolynomial::operator>>=(const unsigned int& n)
{
    if (n <= m_poly.size())
    {
        for (unsigned int i = 0; i <= deg() - n; ++i)
        {
            m_poly[i] = m_poly[i + n];
        }
        m_poly.resize(m_poly.size() - n,GaloisFieldElement(m_gfield,0));
    }
    else if (static_cast<int>(n) >= (deg() + 1))
    {
        m_poly.resize(0,GaloisFieldElement(m_gfield,0));
    }
    return *this;
}


GaloisFieldElement& GaloisFieldPolynomial::operator[](const unsigned int& term)
{
    assert(term < m_poly.size());
    return m_poly[term];
}


GaloisFieldElement GaloisFieldPolynomial::operator()(const GaloisFieldElement& value)
{
    GaloisFieldElement result(m_gfield,0);

    if (!m_poly.empty())
    {
        field_symbol total_sum = 0 ;
        field_symbol value_poly_form = value.poly();
        int i = 0;
        for (PolyIterator it = m_poly.begin(); it != m_poly.end(); ++it, ++i)
        {
            total_sum ^= m_gfield.mul(m_gfield.exp(value_poly_form,i),(*it).poly());
        }
        result = total_sum;
    }
    return result;
}

GaloisFieldElement GaloisFieldPolynomial::operator()(field_symbol value)
{
    if (!m_poly.empty())
    {
        field_symbol total_sum = 0 ;
        int i = 0;
        for (ConstPolyIterator it = m_poly.begin(); it != m_poly.end(); ++it, ++i)
        {
            total_sum ^= m_gfield.mul(m_gfield.exp(value,i),(*it).poly());
        }
        return GaloisFieldElement(m_gfield,total_sum);
    }
    return GaloisFieldElement(m_gfield,0);
}


const GaloisFieldElement& GaloisFieldPolynomial::operator[](const unsigned int& term) const
{
    assert(term < m_poly.size());
    return m_poly[term];
}


const GaloisFieldElement GaloisFieldPolynomial::operator()(const GaloisFieldElement& value) const
{
    if (!m_poly.empty())
    {
        field_symbol total_sum = 0 ;
        field_symbol value_poly_form = value.poly();
        int i = 0;
        for (ConstPolyIterator it = m_poly.begin(); it != m_poly.end(); ++it, ++i)
        {
            total_sum ^= m_gfield.mul(m_gfield.exp(value_poly_form,i),(*it).poly());
        }
        return GaloisFieldElement(m_gfield,total_sum);
    }
    return GaloisFieldElement(m_gfield,0);
}



const GaloisFieldElement GaloisFieldPolynomial::operator()(field_symbol value) const
{
    if (!m_poly.empty())
    {
        field_symbol total_sum = 0 ;
        int i = 0;
        for (ConstPolyIterator it = m_poly.begin(); it != m_poly.end(); ++it, ++i)
        {
            total_sum ^= m_gfield.mul(m_gfield.exp(value,i),(*it).poly());
        }
        return GaloisFieldElement(m_gfield,total_sum);
    }
    return GaloisFieldElement(m_gfield,0);
}

bool GaloisFieldPolynomial::operator == (const GaloisFieldPolynomial& polynomial) const
{
    if (m_gfield == polynomial.m_gfield)
    {
        if (m_poly.size() != polynomial.m_poly.size())
        {
            return false;
        }
        else
        {
            if (m_poly == polynomial.m_poly)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
}

bool GaloisFieldPolynomial::operator!=(const GaloisFieldPolynomial& polynomial) const
{
    return !(*this == polynomial);
}

//bool GaloisFieldPolynomial::monic() const
//{
//    return (poly_[poly_.size() - 1] == static_cast<field_symbol>(1));
//}

GaloisFieldPolynomial GaloisFieldPolynomial::derivative() const
{
    if ((*this).m_poly.size() > 1)
    {
        GaloisFieldPolynomial deriv(m_gfield,deg());
        for (unsigned int i = 0; i < m_poly.size() - 1; ++i)
        {
            if ((i & 1) == 0)
                deriv.m_poly[i] = m_poly[i + 1];
            else
                deriv.m_poly[i] = 0;
        }
        //deriv->simplify();
        return deriv;
    }
    return GaloisFieldPolynomial(m_gfield,0);
}

// -----------------------------------------------------------------------------
// public   
std::string GaloisFieldPolynomial::str() const
{
    std::ostringstream oss;
    for (unsigned int i = 0; i < this->m_poly.size(); ++i)
    {
        oss << this->m_poly[i].poly()
            << "*"<< "x^"<< i
            << ((static_cast<int>(i) != (this->deg())) ? " + " : "");
    }

    return oss.str();
}

// -----------------------------------------------------------------------------
// public   
std::string GaloisFieldPolynomial::simplify_str() const
{

    std::ostringstream oss;

    if (this->valid() == false)
    {
        return oss.str();
    }

    for (unsigned int i = 0; i < this->m_poly.size(); ++i)
    {


        if (this->m_poly[i].poly() != 0)
        {
            if (oss.str().size() != 0)
            {
                oss << " + ";
            }

            oss << this->m_poly[i].poly() << "*x^" << i; 
        }

    }

    if (oss.str().size() == 0)
    {
        return "0";
    }
    else
    {
        return oss.str();
    }
}




//void GaloisFieldPolynomial::simplify(GaloisFieldPolynomial& polynomial) const
//{
//    std::size_t poly_size = polynomial.poly_.size();
//    if ((poly_size > 0) && (polynomial.poly_.back() == 0))
//    {
//        PolyIterator it = polynomial.poly_.end();
//        std::size_t count = 0;
//        while (polynomial.poly_.begin() != it)
//        {
//            if ((*(--it)) == 0)
//                ++count;
//            else
//                break;
//        }
//        if (0 != count)
//        {
//            if (count == poly_size)
//            {
//                polynomial.poly_.resize(poly_size - count + 1, GaloisFieldElement(field_,0));
//            }
//            else
//            {
//                polynomial.poly_.resize(poly_size - count, GaloisFieldElement(field_,0));
//            }
//            
//        }
//    }
//}


 GaloisFieldPolynomial operator + (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
{
    GaloisFieldPolynomial result = a;
    result += b;
    return result;
}


 GaloisFieldPolynomial operator + (const GaloisFieldPolynomial& a, const GaloisFieldElement& b)
{
    GaloisFieldPolynomial result = a;
    result += b;
    return result;
}


 GaloisFieldPolynomial operator + (const GaloisFieldElement& a, const GaloisFieldPolynomial& b)
{
    GaloisFieldPolynomial result = b;
    result += a;
    return result;
}

 GaloisFieldPolynomial operator + (const GaloisFieldPolynomial& a, const field_symbol& b)
{
    return a + GaloisFieldElement(a.galois_field(),b);
}

 GaloisFieldPolynomial operator + (const field_symbol& a, const GaloisFieldPolynomial& b)
{
    return b + GaloisFieldElement(b.galois_field(),a);
}

 GaloisFieldPolynomial operator - (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
{
    GaloisFieldPolynomial result = a;
    result -= b;
    return result;
}

 GaloisFieldPolynomial operator - (const GaloisFieldPolynomial& a, const GaloisFieldElement& b)
{
    GaloisFieldPolynomial result = a;
    result -= b;
    return result;
}

 GaloisFieldPolynomial operator - (const GaloisFieldElement& a, const GaloisFieldPolynomial& b)
{
    GaloisFieldPolynomial result = b;
    result -= a;
    return result;
}

 GaloisFieldPolynomial operator - (const GaloisFieldPolynomial& a, const field_symbol& b)
{
    return a - GaloisFieldElement(a.galois_field(),b);
}

 GaloisFieldPolynomial operator - (const field_symbol& a, const GaloisFieldPolynomial& b)
{
    return b - GaloisFieldElement(b.galois_field(),a);
}


 GaloisFieldPolynomial operator * (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
{
    GaloisFieldPolynomial result = a;
    result *= b;
    return result;
}

 GaloisFieldPolynomial operator * (const GaloisFieldElement& a, const GaloisFieldPolynomial& b)
{
    GaloisFieldPolynomial result = b;
    result *= a;
    return result;
}

 GaloisFieldPolynomial operator * (const GaloisFieldPolynomial& a, const GaloisFieldElement& b)
{
    GaloisFieldPolynomial result = a;
    result *= b;
    return result;
}

 GaloisFieldPolynomial operator / (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
{
    GaloisFieldPolynomial result = a;
    result /= b;
    return result;
}

 GaloisFieldPolynomial operator / (const GaloisFieldPolynomial& a, const GaloisFieldElement& b)
{
    GaloisFieldPolynomial result = a;
    result /= b;
    return result;
}


 GaloisFieldPolynomial operator % (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
{
    GaloisFieldPolynomial result = a;
    result %= b;
    return result;
}


 GaloisFieldPolynomial operator % (const GaloisFieldPolynomial& a, const unsigned int& power)
{
    GaloisFieldPolynomial result = a;
    result %= power;
    return result;
}


 GaloisFieldPolynomial operator ^ (const GaloisFieldPolynomial& a, const int& n)
{
    GaloisFieldPolynomial result = a;
    result ^= n;
    return result;
}

 GaloisFieldPolynomial operator <<(const GaloisFieldPolynomial& a, const unsigned int& n)
{
    GaloisFieldPolynomial result = a;
    result <<= n;
    return result; 
}

 GaloisFieldPolynomial operator >> (const GaloisFieldPolynomial& a, const unsigned int& n)
{
    GaloisFieldPolynomial result = a;
    result >>= n;
    return result;
}

// -----------------------------------------------------------------------------
// public   
//GaloisFieldPolynomial gcd( const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b )
//{
//    if ((a.galois_field()) == (b.galois_field()))
//    {
//        if ((!a.valid()) && (!b.valid()))
//        {
//            GaloisFieldPolynomial error_polynomial(a.galois_field());
//            return error_polynomial;
//        }
//
//        if (!a.valid())
//        {
//            return b;
//        }
//        if (!b.valid())
//        {
//            return a;
//        }
//
//        GaloisFieldPolynomial x = a % b;
//        GaloisFieldPolynomial y = b;
//        GaloisFieldPolynomial z = x;
//
//        while ((z = (y % x)).valid())
//        {
//            y = x;
//            x = z;
//        }
//
//        return x;
//    }
//    else
//    {
//        GaloisFieldPolynomial error_polynomial(a.galois_field());
//        return error_polynomial;
//    }
//}




// -----------------------------------------------------------------------------
// public   
std::ostream& operator<<( std::ostream& os, const GaloisFieldPolynomial& polynomial )
{
    if (polynomial.deg() >= 0)
    {
        /*
        for (unsigned int i = 0; i < polynomial.poly_.size(); ++i)
        {
        os << polynomial.poly[i].index()
        << ((i != (polynomial.deg())) ? " " : "");
        }

        std::cout << " poly form: ";
        */

        for (unsigned int i = 0; i < polynomial.m_poly.size(); ++i)
        {
            os << polynomial.m_poly[i].poly()
                << " "
                << "x^"
                << i
                << ((static_cast<int>(i) != (polynomial.deg())) ? " + " : "");
        }
    }
    return os;
}

GaloisFieldPolynomial generateXValue(const GaloisField& gfield)
{
    GaloisFieldElement xgfe[2] = {
        GaloisFieldElement(gfield, 0), 
        GaloisFieldElement(gfield, 1)
    };

    GaloisFieldPolynomial X_(gfield,1,xgfe);
    return X_;
}



bool generatorPolynomialCreator(const GaloisField& field,
                                const std::size_t initial_index,
                                const std::size_t num_elements,
                                GaloisFieldPolynomial& generator_polynomial)
{
    if (
        (initial_index                 >= field.size()) ||
        ((initial_index + num_elements) >  field.size())
        )
    {
        return false;
    }

    GaloisFieldElement    ALPHA(field,2);
    GaloisFieldPolynomial X  = generateXValue(field);

    generator_polynomial = GaloisFieldElement(field, 1);

    for (std::size_t i = initial_index; i < (initial_index + num_elements); ++i)
    {
        generator_polynomial *= (X + (ALPHA ^ static_cast<field_symbol>(i)));
    }

    generator_polynomial.simplify();

    return  true;
}

// 
// -----------------------------------------------------------------------------