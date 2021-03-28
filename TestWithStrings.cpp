/*
 * \file TestWithStrings.cpp
 * \copyright (C) 2021 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 22.03.2021
 * \time : 12:11
 */

#include <iostream>
#include <string>

#include "CArray.h"
#include "Utils.h"

namespace {
using Vector = G5::CArray< std::string >;

void fillWithRandomStrings( Vector & vector ) {
    constexpr const auto TOTAL_STRINGS_COUNT = 15;

    vector.reserve( TOTAL_STRINGS_COUNT );
    for( int i = 0; i < TOTAL_STRINGS_COUNT; ++i ) {
        vector.push_back( utils::randomString( utils::random( 3, 9 ) ) );
    }
}

void sort( Vector & vector ) {
    std::sort( vector.begin(), vector.end(),
               []( const std::string & lhs, const std::string & rhs )
               {
                   auto res = lhs.compare( rhs );
                   return res > 0;
               } );
}

void filter( Vector & vector ) {

    auto needToRemove = []( const std::string & str ) ->bool
    {
        constexpr char forbiddenChars[] = "abcde";
        return std::any_of( str.begin(), str.end(),
                            [ &forbiddenChars ]( char strSymbol )
                            {
                                return std::any_of( std::begin( forbiddenChars ), std::end( forbiddenChars),
                                                    [ strSymbol ]( char forbidden ){ return strSymbol == forbidden; });
                            } );
    };

    size_t index = 0;
    for( auto it = vector.begin(); it != vector.end(); )
    {
        if( needToRemove( *it ) ) {
            it = vector.erase( index );
        }
        else {
            ++it;
            ++index;
        }
    }
}

void insertInRandomPositions( Vector & vector ) {
    for( int i = 0; i < 3; ++i ) {
        auto str = utils::randomString( utils::random( 3, 15 ) );
        auto max = vector.size() > 1 ? vector.size() - 1 : 1;
        auto insertPos = utils::random( 0, static_cast<int>(max) );

        vector.insert( insertPos, str );
    }
};

}

void testWithStrings() noexcept
{
    try
    {
        Vector vector;

        fillWithRandomStrings( vector );
        utils::printVector( vector );

        sort( vector );
        utils::printVector( vector );

        filter( vector );
        utils::printVector( vector );

        insertInRandomPositions( vector );
        utils::printVector( vector );
    }
    catch ( const std::exception & error ) { std::cerr << error.what() << '\n'; }
}