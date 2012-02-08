/*
 * Instruction.hpp
 *
 *  Created on: 7 Feb 2012
 *      Author: Max Foster
 */

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Token.hpp"

struct Instruction
{
    Token label, opcode, operand1, operand2;

    Instruction() {}
    Instruction(const Token & label, const Token & opcode, const Token & operand1, const Token & operand2)
        : label(label), opcode(opcode), operand1(operand1), operand2(operand2) {}

    void clear() { label.clear(); opcode.clear(); operand1.clear(); operand2.clear(); }
};

#endif // INSTRUCTION_HPP
