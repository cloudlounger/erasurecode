/******************************************************************************* 
 *  @file      GaloisField.cpp 2013\8\8 17:43:20 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com>
 ******************************************************************************/

#include "GaloisField.h"

/******************************************************************************/

// -----------------------------------------------------------------------------
// public   
GaloisField::GaloisField( const unsigned int pwr, 
                          const unsigned int* primitive_poly, 
                          const unsigned int primpoly_deg )
    : power_(pwr)
    , prim_poly_deg_(primpoly_deg)
    , field_size_((1 << pwr) - 1)
    , prim_poly_hash_(0xAAAAAAAA)
    , prim_poly_(primitive_poly)
    , alpha_to_(0)
    , index_of_(0)
#if !defined(DYNAMIC_COMPUTER)
    , mul_inverse_(0)
    , mul_table_(0)
    , div_table_(0)
    , exp_table_(0)
    , buffer_(0)
#endif//DYNAMIC_COMPUTER
{
    alpha_to_    = new field_symbol [field_size_ + 1];
    index_of_    = new field_symbol [field_size_ + 1];

#if !defined(DYNAMIC_COMPUTER)
    const std::size_t buffer_size = ((4 * (field_size_ + 1) * (field_size_ + 1)) + ((field_size_ + 1) * 2)) * sizeof(field_symbol);
    buffer_ = new char[buffer_size];
    std::size_t offset = 0;
    offset = create_2d_array(buffer_, (field_size_ + 1), (field_size_ + 1), offset, &mul_table_);
    offset = create_2d_array(buffer_, (field_size_ + 1), (field_size_ + 1), offset, &div_table_);
    offset = create_2d_array(buffer_, (field_size_ + 1), (field_size_ + 1), offset, &exp_table_);
    offset = create_array(buffer_, (field_size_ + 1) * 2, offset, &mul_inverse_);
#endif


    // computer hash valuse
    prim_poly_hash_ = 0xAAAAAAAA;
    for (std::size_t i = 0; i < (prim_poly_deg_ + 1); ++i)
    {
        prim_poly_hash_ += ((i & 1) == 0) 
            ? (  (prim_poly_hash_ <<  7) ^  primitive_poly[i] * (prim_poly_hash_ >> 3)) 
            : (~((prim_poly_hash_ << 11) + (primitive_poly[i] ^ (prim_poly_hash_ >> 5))));
    } 

    generate_field();
}

// -----------------------------------------------------------------------------
// public   
GaloisField::~GaloisField()
{
    delete[] index_of_;       index_of_ = 0;
    delete[] alpha_to_;       alpha_to_ = 0;
#if !defined(DYNAMIC_COMPUTER)
    mul_inverse_ = 0;
    delete[] exp_table_;   exp_table_   = 0;
    delete[] div_table_;   div_table_   = 0;
    delete[] mul_table_;   mul_table_   = 0;
    delete[] buffer_;      buffer_      = 0;
#endif
}


field_symbol GaloisField::index(const field_symbol value) const
{
    return index_of_[value];
}

field_symbol GaloisField::alpha(const field_symbol value) const
{
    return alpha_to_[value];
}

unsigned int GaloisField::size() const
{
    return field_size_;
}

unsigned int GaloisField::pwr() const
{
    return power_;
}

unsigned int GaloisField::mask() const
{
    return field_size_;
}


field_symbol GaloisField::add(const field_symbol& a, const field_symbol& b) const
{
    return (a ^ b);
}

field_symbol GaloisField::sub(const field_symbol& a, const field_symbol& b) const
{
    return (a ^ b);
}


// -----------------------------------------------------------------------------
// public   
bool GaloisField::operator==( const GaloisField& gf ) const
{
    return ((this->power_ == gf.power_) && (this->prim_poly_hash_ == gf.prim_poly_hash_));
}

// -----------------------------------------------------------------------------
// public   
bool GaloisField::operator!=( const GaloisField& gf ) const
{
    return (*this == gf);
}


// -----------------------------------------------------------------------------
// public   
field_symbol GaloisField::mul( const field_symbol& a, const field_symbol& b ) const
{
#if !defined(DYNAMIC_COMPUTER)
    return mul_table_[a][b];
#else
    if ((a == 0) || (b == 0))
        return 0;
    else
    {
        return alpha_to_[normalize(index_of_[a] + index_of_[b])];
    }
#endif
}

// -----------------------------------------------------------------------------
// public   
field_symbol GaloisField::div( const field_symbol& a, const field_symbol& b ) const
{
#if !defined(DYNAMIC_COMPUTER)
    return div_table_[a][b];
#else
    if ((a == 0) || (b == 0))
        return 0;
    else
        return alpha_to_[normalize(index_of_[a] - index_of_[b] + field_size_)];
#endif
}

// -----------------------------------------------------------------------------
// public   
field_symbol GaloisField::exp( const field_symbol& a, const std::size_t& n ) const
{
#if !defined(DYNAMIC_COMPUTER)
    if (n < 0)
    {
        int b = n;
        while (b < 0) 
        {
            b += field_size_;
        }

        return ((b == 0) ? 1 : exp_table_[a][b]);
    }
    else
        return exp_table_[a][n & field_size_];
#else
    if (a != 0)
    {
        if (n < 0)
        {
            int b = n;
            while (b < 0) 
            {
                b += field_size_;
            }
            return ((b == 0) ? 1 : alpha_to_[normalize(index_of_[a] * b)]);
        }
        else if (n == 0)
            return 1;
        else
            return alpha_to_[normalize(index_of_[a] * static_cast<field_symbol>(n))];
    }
    else
        return 0;
#endif
}


// -----------------------------------------------------------------------------
// public   
field_symbol GaloisField::inverse( const field_symbol& val ) const
{
#if !defined(DYNAMIC_COMPUTER)
    return mul_inverse_[val];
#else
    return alpha_to_[normalize(field_size_ - index_of_[val])];
#endif
}

// -----------------------------------------------------------------------------
// public   
field_symbol GaloisField::normalize( field_symbol x ) const
{
    while (x < 0)
    {
        x += static_cast<field_symbol>(field_size_);
    }

    while (x >= static_cast<field_symbol>(field_size_))
    {
        x -= static_cast<field_symbol>(field_size_);
        x  = (x >> power_) + (x & field_size_);
    }

    return x;
}


// -----------------------------------------------------------------------------
// private   
void GaloisField::generate_field()
{
    field_symbol mask = 1;

    alpha_to_[power_] = 0;

    for (field_symbol i = 0; i < static_cast<field_symbol>(power_); ++i)
    {
        alpha_to_[i]           = mask;
        index_of_[alpha_to_[i]] = i;

        if (prim_poly_[i] != 0)
        {
            alpha_to_[power_] ^= mask;
        }

        mask <<= 1;
    }

    index_of_[alpha_to_[power_]] = power_;

    mask >>= 1;

    for (field_symbol i = power_ + 1; i < static_cast<field_symbol>(field_size_); ++i)
    {
        if (alpha_to_[i - 1] >= mask)
        {
            alpha_to_[i] = alpha_to_[power_] ^ ((alpha_to_[i - 1] ^ mask) << 1);
        }
        else
        {
            alpha_to_[i] = alpha_to_[i - 1] << 1;
        }

        index_of_[alpha_to_[i]] = i;
    }

    index_of_[0] = GFERROR;
    alpha_to_[field_size_] = 1;


#if !defined(DYNAMIC_COMPUTER)

    for (field_symbol i = 0; i < static_cast<field_symbol>(field_size_ + 1); ++i)
    {
        for (field_symbol j = 0; j < static_cast<field_symbol>(field_size_ + 1); ++j)
        {
            mul_table_[i][j] = gen_mul(i,j);
            div_table_[i][j] = gen_div(i,j);
            exp_table_[i][j] = gen_exp(i,j);
        }
    }

    for (field_symbol i = 0; i < static_cast<field_symbol>(field_size_ + 1); ++i)
    {
        mul_inverse_[i] = gen_inverse(i);
        mul_inverse_[i + (field_size_ + 1)] = mul_inverse_[i];
    }

#endif


}

// -----------------------------------------------------------------------------
// private   
field_symbol GaloisField::gen_mul( const field_symbol& a, const field_symbol& b ) const
{
    if ((a == 0) || (b == 0))
    {
        return 0;
    }
    else
    {
        return alpha_to_[normalize(index_of_[a] + index_of_[b])];
    }
}

// -----------------------------------------------------------------------------
// private   
field_symbol GaloisField::gen_div( const field_symbol& a, const field_symbol& b ) const
{
    if ((a == 0) || (b == 0))
    {
        return 0;
    }
    else
    {
        return alpha_to_[normalize(index_of_[a] - index_of_[b] + field_size_)];
    }
}

// -----------------------------------------------------------------------------
// private   
field_symbol GaloisField::gen_exp( const field_symbol& a, const std::size_t& n ) const
{
    if (a != 0)
    {
        return ((n == 0) ? 1 : alpha_to_[normalize(index_of_[a] * static_cast<field_symbol>(n))]);
    }
    else
    {
        return 0;
    }
}

// -----------------------------------------------------------------------------
// private   
field_symbol GaloisField::gen_inverse( const field_symbol& val ) const
{
    return alpha_to_[normalize(field_size_ - index_of_[val])];
}

// -----------------------------------------------------------------------------
// private 
std::size_t GaloisField::create_array( char buffer_[], 
    const std::size_t& length, 
    const std::size_t offset, 
    field_symbol** array )
{
    const std::size_t row_size = length * sizeof(field_symbol);
    (*array) = (field_symbol*)(buffer_ + offset);
    return row_size + offset;
}


// -----------------------------------------------------------------------------
// private
std::size_t GaloisField::create_2d_array( char buffer_[], 
    std::size_t row_cnt, 
    std::size_t col_cnt, 
    const std::size_t offset, 
    field_symbol*** array )
{

    const std::size_t row_size = col_cnt * sizeof(field_symbol);
    char* buffer__offset = buffer_ + offset;
    (*array) = new field_symbol* [row_cnt];
    for (std::size_t i = 0; i < row_cnt; ++i)
    {
        (*array)[i] = new(buffer__offset + (i * row_size))field_symbol[col_cnt];
    }
    return (row_cnt * row_size) + offset;


}


std::ostream& operator << (std::ostream& os, const GaloisField& gf)
{
    for (std::size_t i = 0; i < (gf.size()+ 1); ++i)
    {
        os << i << "\t" << gf.alpha(i) << "\t" << gf.index(i) << std::endl;
    }

    return os;
}

// 
// -----------------------------------------------------------------------------
