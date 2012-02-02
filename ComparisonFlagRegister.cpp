/*
 * Flags.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include "ComparisonFlagRegister.hpp"

ComparisonFlagRegister::ComparisonFlagRegister()
{
    reset();
}

bool ComparisonFlagRegister::getValue(const ComparisonFlagRegister::ComparisonFlagId id) const
{
    return flags[id];
}

void ComparisonFlagRegister::setValue(const ComparisonFlagRegister::ComparisonFlagId id, const bool value)
{
    flags[id] = value;
}

void ComparisonFlagRegister::toggle(const ComparisonFlagRegister::ComparisonFlagId id)
{
    flags[id] = !flags[id];
}

void ComparisonFlagRegister::reset()
{
    for (int i = 0; i < FLAG_COUNT; ++i) flags[i] = false;
}
