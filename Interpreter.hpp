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
    enum Option
    {
        O_TIME_EXECUTION = 0,
        OPTION_COUNT
    };

    // Run in command line mode
    Interpreter(Machine & machine, unsigned optionCount, const Option * options);
    // Run from file (just plain text for now)
    Interpreter(Machine & machine, const char * fileName, unsigned optionCount, const Option * options);

    void parseOptions(unsigned optionCount, const Option * options);

    void tokenizeAndAddInstruction(const std::string & instruction, unsigned line);
    void preOptimise(); // Optimisation that occurs before running the program
    void run();
    void runWithoutOptions();
    void outputTokenData(const std::string & instruction);
    void execute(const Instruction & instruction);

private:
    static const unsigned instructionReservation = 10000;

    bool optionEnabled[OPTION_COUNT];
    Machine & machine;
    std::vector<Instruction> instructions;

    Block * getBlockFromToken(const Token & token, bool & isLabel, const short operandNumber);
};

#endif // INTERPRETER_HPP
