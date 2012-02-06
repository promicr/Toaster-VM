/*
 * ComparisonFlagRegister.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef COMPARISONFLAGREGISTER_HPP
#define COMPARISONFLAGREGISTER_HPP

// A class sotring flags which are used for storing the results of comparison operations

typedef class ComparisonFlagRegister
{
public:
    enum ComparisonFlagId
    {
        F_EQUAL = 0,
        F_NOT_EQUAL,
        F_LESS,
        F_GREATER,
        F_LESS_EQUAL,
        F_GREATER_EQUAL,
        FLAG_COUNT
    };

    ComparisonFlagRegister();

    bool getValue(ComparisonFlagId id) const;
    void setValue(ComparisonFlagId id, bool value);

    void toggle(ComparisonFlagId id);
    void reset();

private:
    bool flags[FLAG_COUNT];
} CFR;

#endif // COMPARISONFLAGREGISTER_HPP
