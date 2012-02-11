/*
 * Interpreter.hpp
 *
 *  Created on: 5 Feb 2012
 *      Author: Max Foster
 */

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <sstream>
#include <vector>

#include "Instruction.hpp"

class Machine;
class Block;

class Interpreter
{
public:
    Interpreter(Machine & machine); // Run in command line mode
    Interpreter(Machine & machine, const char * fileName); // Run from file (just plain text for now)

    void tokenizeAndAddInstruction(const std::string & instruction, unsigned line);
    void run();
    void outputTokenData(const std::string & instruction);
    void execute(const Instruction & instruction);

private:
    static const unsigned instructionReservation = 10000;

    Machine & machine;
    std::vector<Instruction> instructions;

    Block * getBlockFromToken(const Token & token, bool & isLabel, const short operandNumber);
};

#endif // INTERPRETER_HPP
