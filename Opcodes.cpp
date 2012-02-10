/*
 * Opcodes.cpp
 *
 *  Created on: 10 Feb 2012
 *      Author: Max Foster
 */

#include "Opcodes.hpp"

int Opcodes::getOpcodeId(const std::string & opcode)
{
    int i = 0;
    while (true)
    {
        if (opcodeStrings[i] == "#") return -1;
        if (opcodeStrings[i] == opcode) return i;
        ++i;
    }
}
