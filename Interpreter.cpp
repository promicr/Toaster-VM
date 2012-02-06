/*
 * Interpreter.cpp
 *
 *  Created on: 5 Feb 2012
 *      Author: Max Foster
 */

#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>

#include "Interpreter.hpp"
#include "Machine.hpp"

const std::string Interpreter::opcodes[] =
{ "clr", "set", "move", "swap", "in", "ins", "out", "outs", "push", "pop", "inc", "dec", "neg", "abs", "add", "sub",
  "mul", "div", "mod", "sadd", "ssub", "smul", "sdiv", "allc", "pla", "fna" , "atoa", "ael", "alen", "cnvi", "cnvr",
  "cnvc", "cnvb", "cnvt", "dref", "cmp", "cmpt", "ist", "cpyf", "not", "and", "or", "xor", "jump", "je", "jne", "jl",
  "jg", "jle", "jge", "call", "ret", "extl", "extc", "#" };

int Interpreter::opcodeId(const std::string & opcode)
{
    int i = 0;
    while (true)
    {
        if (opcodes[i] == "#") return -1;
        if (opcodes[i] == opcode) return i;
        ++i;
    }
}

Interpreter::Interpreter(Machine & machine)
    : machine(machine), runningFromFile(false) {}

Interpreter::Interpreter(Machine & machine, const char * fileName)
    : machine(machine), runningFromFile(true)
{
    if ((fileName == NULL) || (strlen(fileName) == 0))
        throw(std::runtime_error("Parser::Parser: Invalid file name given"));

    std::fstream file(fileName, std::ios_base::in);
    if (file.is_open())
    {
        fileContents << file.rdbuf();
        file.close();
    }
    else throw(std::runtime_error("Parser::Parser: File could not be opened"));
}

void Interpreter::run()
{
    unsigned line = 0;
    std::string instruction;
    if (runningFromFile)
    {
        while (true)
        {
            ++line;
            getline(fileContents, instruction);
            if (fileContents.eof()) break;

            try { execute(instruction); }
            catch (const std::exception & e)
            {
                std::cout << "Error on line " << line << std::endl
                          << instruction << std::endl
                          << e.what() << std::endl
                          << "Execution halted" << std::endl;
                break;
            }
        }
    }
    else
    {
        while (true)
        {
            ++line;
            std::cout << line << " >> ";
            getline(std::cin, instruction);
            if (instruction == "EXIT") break;
            try { execute(instruction); } catch (const std::exception & e) { std::cout << e.what() << std::endl; }
        }
    }
}

void Interpreter::execute(const std::string & instruction)
{

}
