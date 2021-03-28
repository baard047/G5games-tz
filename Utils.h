/*
 * \file Utils.h
 * \copyright (C) 2021 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 28.03.2021
 * \time : 12:38
 */

#pragma once

#include <random>
#include <stdexcept>
#include <iterator>
#include <string>
#include <algorithm>
#include <type_traits>
#include <numeric>

#include "CArray.h"

namespace utils {

namespace SFINAE {

template< class T >
using IsSigned = typename std::enable_if_t< std::is_signed< T >::value
                                            && !std::is_floating_point< T >::value
                                            && !std::is_same< T, bool >::value >;

template< class T >
using IsFloatingPoint = typename std::enable_if_t< std::is_floating_point< T >::value >;

template< class T >
using IsString = typename std::enable_if_t< std::is_convertible< T, std::string >::value >;

}

template< class DigitT, SFINAE::IsSigned<DigitT> * = nullptr >
auto random( DigitT min, DigitT max ) {
    if( min == max ) { return min; }
    else if( min > max ) { throw std::invalid_argument("Invalid range!"); }

    std::random_device device;
    std::mt19937 mt(device());
    std::uniform_int_distribution<DigitT> seed(min, max);
    return seed(mt);
}

template< class DigitT, SFINAE::IsFloatingPoint<DigitT> * = nullptr >
auto random( DigitT min, DigitT max ) {
    if( min == max ) { return min; }
    else if( min > max ) { throw std::invalid_argument("Invalid range!"); }

    std::random_device device;
    std::mt19937 mt(device());
    std::uniform_real_distribution<DigitT> seed(min, max);
    return seed(mt);
}

inline std::string randomString( size_t length )
{
    auto randomSymbol = []() -> char
    {
        static constexpr char characters[] = "abcdefghijklmnopqrstuvwxyz";
        return characters[ random(0, (int)sizeof( characters ) - 1) ];
    };

    std::string res( length, 0 );
    std::generate( res.begin(), res.end(), randomSymbol );
    return res;
}

template< class DataT >
void printVector( const G5::CArray< DataT > & vector ) {
    if( vector.empty() ) {
        std::cout << "vector is empty!";
    }
    else {
        std::cout << "size: " << vector.size() << '\n';
        std::copy( vector.begin(), vector.end(), std::ostream_iterator<DataT>( std::cout, " " ) );
    }
    std::cout << "\n-----------------------------------------------------------------------------\n";
}

}
