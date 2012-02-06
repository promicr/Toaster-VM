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

class Machine;

class Interpreter
{
public:
    static int opcodeId(const std::string & opcode);

    Interpreter(Machine & machine); // Run in command line mode
    Interpreter(Machine & machine, const char * fileName); // Run from file (just plain text for now)

    void run();
    void execute(const std::string & instruction);

private:
    static const std::string opcodes[];

    Machine & machine;
    bool runningFromFile;
    std::stringstream fileContents;
};

#endif // INTERPRETER_HPP
