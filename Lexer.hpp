/*
 * Lexer.hpp
 *
 *  Created on: 7 Feb 2012
 *      Author: Max Foster
 */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>

class Instruction;
class Token;

class Lexer
{
public:
    static const std::string opcodeStrings[];

    static int getOpcodeId(const std::string & opcode);
    static const Instruction & tokenize(const std::string & instruction);

    static Token getOpcodeTokenFrom(const std::string & str);
    static Token getOperandTokenFrom(const std::string & str);

private:
    Lexer() {} // To stop instantiation of this class
};

#endif // LEXER_HPP
