/*
 * Parser.cpp
 *
 *  Created on: 5 Feb 2012
 *      Author: Max Foster
 */

#include "Parser.hpp"

const std::string Parser::opcodes[] =
{ "clr", "set", "move", "swap", "in", "ins", "out", "outs", "push", "pop", "inc", "dec", "neg" "add", "sub", "mul",
  "div", "sadd", "ssub", "smul", "sdiv", "allc", "pla", "fna" , "atoa", "ael", "alen", "cnvi", "cnvr", "cnvc", "cnvb",
  "cnvt", "dref", "cmp", "cmpt", "ist", "cpyf", "not", "and", "or", "xor", "jump", "je", "jne", "jl", "jg", "jle",
  "jge", "call", "ret", "extl", "extc", "#" };

int Parser::opcodeId(const std::string & opcode)
{
    int i = 0;
    while (true)
    {
        if (opcodes[i] == "#") return -1;
        if (opcodes[i] == opcode) return i;
        ++i;
    }
}
