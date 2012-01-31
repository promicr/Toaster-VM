/*
 * Flags.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef FLAGS_HPP
#define FLAGS_HPP

// Flags which are used for storing the results of comparison operations

namespace Flags
{

enum Id
{
    F_EQUAL = 0,
    F_NOT_EQUAL,
    F_LESS,
    F_GREATER,
    F_LESS_EQUAL,
    F_GREATER_EQUAL,
    FLAG_COUNT
};

void init();

bool getValue(Flags::Id flagId);
void setValue(Flags::Id flagId, bool value);

void Toggle(Flags::Id flagId);

}

#endif // FLAGS_HPP
