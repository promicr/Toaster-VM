/*
 * Flags.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef FLAGS_HPP
#define FLAGS_HPP

// A class sotring flags which are used for storing the results of comparison operations

class FlagRegister
{
public:
    enum FlagId
    {
        F_EQUAL = 0,
        F_NOT_EQUAL,
        F_LESS,
        F_GREATER,
        F_LESS_EQUAL,
        F_GREATER_EQUAL,
        FLAG_COUNT
    };

    FlagRegister();

    bool getValue(FlagId id);
    void setValue(FlagId id, bool value);

    void Toggle(FlagId id);

private:
    bool flags[FLAG_COUNT];
};

#endif // FLAGS_HPP
