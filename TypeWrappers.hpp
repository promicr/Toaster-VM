/*
 * TypeWrappers.hpp
 *
 *  Created on: 7 Feb 2012
 *      Author: Max Foster
 */

#ifndef TYPEWRAPPERS_HPP
#define TYPEWRAPPERS_HPP

#include <cstring>

// Wrappers for data types so that overloading works without compiler complaining about ambiguities

struct Integer
{
    long value;
    explicit Integer(const long value) : value(value) {}
    operator long() const { return value; }
    Integer & operator =(const long other) { value = other; return *this; }
};

struct Real
{
    double value;
    explicit Real(const double value) : value(value) {}
    operator double() const { return value; }
    Real & operator =(const double other) { value = other; return *this; }
};

struct Char
{
    char value;
    explicit Char(const char value) : value(value) {}
    operator char() const { return value; }
    Char & operator =(const char other) { value = other; return *this; }
};

struct Boolean
{
    bool value;
    explicit Boolean(const bool value) : value(value) {}
    operator bool() const { return value; }
    Boolean & operator =(const bool other) { value = other; return *this; }
};

struct Label
{
    static const unsigned length = 12;

    char value[length + 1];
    unsigned line;
    explicit Label(const char * value_, const unsigned line) : line(line) { strncpy(value, value_, length); }
};

struct StackLocation
{
    unsigned value;
    explicit StackLocation(const unsigned value) : value(value) {}
    operator unsigned() const { return value; }
    StackLocation & operator =(const unsigned rhs) { value = rhs; return *this; }
};

struct HeapLocation
{
    unsigned value;
    explicit HeapLocation(const unsigned value) : value(value) {}
    operator unsigned() const { return value; }
    HeapLocation & operator =(const unsigned rhs) { value = rhs; return *this; }
};

#endif // TYPEWRAPPERS_HPP
