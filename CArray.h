/*
 * \file CArray.h
 * \copyright (C) 2021 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 27.03.2021
 * \time : 19:11
 */

#pragma once

#include <cstdint>
#include <stdexcept>
#include <memory>
#include <algorithm>

namespace G5 {

template< class TData >
class CArray
{
    using AllocatorTraits = std::allocator_traits< std::allocator< TData > >;

public:
    using value_type = typename AllocatorTraits::value_type;
    using pointer = typename AllocatorTraits::pointer;
    using const_pointer = typename AllocatorTraits::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = uint32_t;

    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator< iterator >;
    using const_reverse_iterator = std::reverse_iterator< const_iterator >;

public:
    CArray() { reallocate( 2 ); }

    ~CArray() {
        clear();
        deleteStorage();
    }

    CArray( const CArray & _other ) {
        operator=( _other );
    }

    CArray( CArray && _other ) noexcept
            : m_data( std::exchange( _other.m_data, m_data ) ),
              m_size( std::exchange( _other.m_size, m_size ) ),
              m_capacity( std::exchange( _other.m_capacity, m_capacity ) )
    {
    }

    CArray & operator =( CArray && _other ) noexcept {
        if( this != &_other ) {
            clear();
            deleteStorage();

            m_data = std::exchange( _other.m_data, nullptr );
            m_size = std::exchange( _other.m_size, 0 );
            m_capacity = std::exchange( _other.m_capacity, 0 );
        }
        return *this;
    }

    CArray & operator =( const CArray & _other ) {
        m_size = _other.m_size;
        m_capacity = _other.m_capacity;
        reallocate( m_capacity );
        std::copy( _other.begin(), _other.end(), begin() );

        return *this;
    }

    void push_back( const value_type & _value ) {
        reallocateIfNeeded();
        m_data[ m_size++ ] = _value;
    }

    void push_back( value_type && _value ) {
        emplace_back( std::move( _value ) );
    }

    template< class ... Args >
    void emplace_back( Args && ... args ) {
        reallocateIfNeeded();
        createAt( m_data, m_size++, std::forward< Args >( args )... );
    }

    void insert( size_type _index, const value_type & _value ) {
        if( _index == 0 || _index == m_size ) {
            push_back( _value );
            return;
        }

        if( m_size >= m_capacity ) {
            auto newCapacity = ++m_capacity;

            auto buffer = createBuffer( newCapacity );
            for( size_type i = 0; i < _index; ++i ) {
                createAt( buffer, i, std::move( m_data[ i ] ) );
                destruct( i );
            }
            for( size_type i = _index; i < m_size; ++i ) {
                createAt( buffer, i + 1, std::move( m_data[ i ] ) );
                destruct(i);
            }

            deleteStorage();
            m_data = buffer;
            m_capacity = newCapacity;
        }
        else
        {
            for( int i = m_size; i > _index && i > 0; --i ) {
                createAt( m_data, i, std::move( m_data[ i - 1 ] ) );
                destruct( i );
            }
        }

        m_data[ _index ] = _value;
        ++m_size;
    }

    iterator erase( size_type _index ) {
        if( _index == m_size - 1 ) {
            pop_back();
            return end();
        }

        destruct( _index );

        for( size_type i = _index; i < size() - 1; ++i ) {
            createAt( m_data, i, std::move( m_data[ i + 1 ] ) );
        }

        --m_size;
        return begin() + _index;
    }

    void reserve( size_type _capacity ) {
        if( capacity() < _capacity )
            reallocate( _capacity );
    }

    void pop_back() {
        if( m_size > 0 )
            destruct( --m_size );
    }

    void clear() {
        for( size_type i = 0; i < m_size; ++i ) {
            destruct( i );
        }
        m_size = 0;
    }

    size_type size() const { return m_size; }
    bool empty() const { return size() == 0; }
    size_type capacity() const { return m_capacity; }

    pointer data() { return m_data; }
    const_pointer data() const { return m_data; }

    reference operator[]( size_type _i ) { return data()[_i]; }
    const_reference operator[]( size_type _i ) const { return data()[_i]; }

    reference at( size_type _i ) {
        if( _i >= size() )
            throw std::out_of_range( "Out of range" );

        return data()[_i];
    }

    const_reference at( size_type _i ) const {
        if( _i >= size() )
            throw std::out_of_range( "Out of range" );

        return data()[_i];
    }

    reference front() { return data()[0]; }
    const_reference front() const { return data()[0]; }

    reference back() { return data()[ size() - 1 ]; }
    const_reference back() const { return data()[ size() - 1 ]; }

    iterator begin() { return data(); }
    const_iterator begin() const { return data(); }
    const_iterator cbegin() const { return begin(); }

    iterator end() { return data() + size(); }
    const_iterator end() const { return data() + size(); }
    const_iterator cend() const { return end(); }

    reverse_iterator rbegin() { return reverse_iterator{ end() }; }
    const_reverse_iterator crbegin() const { return rbegin(); }
    reverse_iterator rend() { return reverse_iterator{ begin() }; }
    const_reverse_iterator crend() const { return rend(); }

private:
    void reallocateIfNeeded() {
        if( m_size >= m_capacity )
            reallocate( m_capacity + m_capacity / 2 );
    }

    void reallocate( size_type _capacity ) {
        auto buffer = createBuffer( _capacity );
        if( m_data != nullptr )
        {
            for( size_type i = 0; i < size(); ++i ) {
                createAt( buffer, i, std::move( m_data[ i ] ) );
                destruct( i );
            }
            deleteStorage();
        }

        m_data = buffer;
        m_capacity = _capacity;
    }

    template< class ... Args >
    void createAt( value_type * _destination, size_type _index, Args && ... args ) {
        new( &_destination[ _index ] ) value_type{ std::forward< Args >( args )... };
    }

    void destruct( size_type _index ) {
        m_data[ _index ].~value_type();
    }

    value_type * createBuffer( size_type _size ) {
        return static_cast< value_type * >( ::operator new( _size * sizeof( value_type ) ) );
    }

    void deleteStorage() {
        ::operator delete( m_data, m_capacity * sizeof( value_type ) );
    }

protected:
    value_type * m_data = nullptr;
    size_type m_size = 0;
    size_type m_capacity = 0;
};

}