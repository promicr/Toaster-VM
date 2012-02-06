/*
 * Parser.hpp
 *
 *  Created on: 5 Feb 2012
 *      Author: Max Foster
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>

class Parser
{
public:
    static int opcodeId(const std::string & opcode);

private:
    static const std::string opcodes[];
};

#endif // PARSER_HPP
