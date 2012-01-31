/*
 * Flags.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include "Flags.hpp"

bool flags[Flags::FLAG_COUNT];

void Flags::init()
{
    for (int i = 0; i < Flags::FLAG_COUNT; ++i) flags[i] = false;
}

bool Flags::getValue(const Flags::Id flagId)
{
    return flags[flagId];
}

void Flags::setValue(const Flags::Id flagId, const bool value)
{
    flags[flagId] = value;
}

void Flags::Toggle(const Flags::Id flagId)
{
    flags[flagId] = !flags[flagName];
}
