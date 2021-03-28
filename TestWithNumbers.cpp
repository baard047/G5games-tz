/*
 * \file TestWithNumbers.cpp
 * \copyright (C) 2021 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 22.03.2021
 * \time : 12:11
 */

#include <iostream>

#include "CArray.h"
#include "Utils.h"

namespace {
using Vector = G5::CArray< int >;

void fillWithRandomNumbers( Vector & vector ) {
    constexpr const auto TOTAL_NUMBERS_COUNT = 20;

    vector.reserve( TOTAL_NUMBERS_COUNT );
    for( int i = 0; i < TOTAL_NUMBERS_COUNT; ++i ) {
        vector.emplace_back( utils::random( 0, 100) );
    }
}

void sort( Vector & vector ) {
    std::sort( vector.begin(), vector.end(), std::less<>{} );
}

void removeEverySecondElement( Vector & vector ) {
    auto halfSize = vector.size() / 2;
    for( decltype( halfSize ) i = 0; i < halfSize; ++i ) {
        vector.erase( i );
    }
}

void insertInRandomPositions( Vector & vector ) {
    for( int i = 0; i < 10; ++i ) {
        auto insertPos = utils::random(0, static_cast<int>( vector.size() ) - 1 );
        vector.insert( insertPos, utils::random( 0, 100) );
    }
};

}

void testWithIntegers() noexcept
{
    try
    {
        Vector vector;

        fillWithRandomNumbers( vector );
        utils::printVector( vector );

        sort( vector );
        utils::printVector( vector );

        removeEverySecondElement( vector );
        utils::printVector( vector );

        insertInRandomPositions( vector );
        utils::printVector( vector );

        vector.clear();
        utils::printVector( vector );
    }
    catch ( const std::exception & error ) { std::cerr << error.what() << '\n'; }
}