/*
 * Flags.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include "Flags.hpp"

FlagRegister::FlagRegister()
{
    for (int i = 0; i < FLAG_COUNT; ++i) flags[i] = false;
}

bool FlagRegister::getValue(const FlagRegister::FlagId id) const
{
    return flags[id];
}

void FlagRegister::setValue(const FlagRegister::FlagId id, const bool value)
{
    flags[id] = value;
}

void FlagRegister::Toggle(const FlagRegister::FlagId id)
{
    flags[id] = !flags[id];
}
