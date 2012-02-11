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
    explicit Integer(const long value) : value(value) {}
    long value;
    operator long() const { return value; }
    Integer & operator =(const long other) { value = other; return *this; }
};

struct Real
{
    explicit Real(const double value) : value(value) {}
    double value;
    operator double() const { return value; }
    Real & operator =(const double other) { value = other; return *this; }
};

struct Char
{
    explicit Char(const char value) : value(value) {}
    char value;
    operator char() const { return value; }
    Char & operator =(const char other) { value = other; return *this; }
};

struct Boolean
{
    explicit Boolean(const bool value) : value(value) {}
    bool value;
    operator bool() const { return value; }
    Boolean & operator =(const bool other) { value = other; return *this; }
};

struct Label
{
    static const unsigned length = 12;

    explicit Label(const char * name_, const unsigned line) : line(line) { strncpy(name, name_, length); }
    char name[length + 1];
    unsigned line;
};

#endif // TYPEWRAPPERS_HPP
