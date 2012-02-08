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
#include <sstream>

#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Instruction.hpp"
#include "Machine.hpp"

Interpreter::Interpreter(Machine & machine)
    : machine(machine)
{
    unsigned line = 0;
    while (true)
    {
        ++line;
        std::cout << line << " >> ";
        std::string buffer;
        getline(std::cin, buffer);
        if (buffer == "END") break;
        fileContents << buffer << '\n';
    }
}

Interpreter::Interpreter(Machine & machine, const char * fileName)
    : machine(machine)
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
    getline(std::cin, instruction);
}

// temporary debug stuff
std::string typeString(const Token::Type type)
{
    switch (type)
    {
    case Token::T_OPCODE: return "opcode";
    case Token::T_OPERAND_CONST_INT: return "const-int";
    case Token::T_OPERAND_CONST_REAL: return "const-real";
    case Token::T_OPERAND_CONST_CHAR: return "const-char";
    case Token::T_OPERAND_CONST_BOOL: return "const-bool";
    case Token::T_OPERAND_STACK_TOP: return "stack-top";
    case Token::T_OPERAND_STACK_BOTTOM: return "stack-bottom";
    case Token::T_OPERAND_HEAP_LOCATION: return "heap-location";
    case Token::T_OPERAND_PRIMARY_REGISTER: return "primary-register";
    case Token::T_OPERAND_MANAGED_OUT_REGISTER: return "managed-register";
    case Token::T_LABEL: return "label";
    case Token::T_NULL: return "null";
    default: return "ERROR";
    }
}

std::string valueString(const Token & token)
{
    std::stringstream stream;
    switch (token.type())
    {
    case Token::T_OPCODE: stream << (int)token.opcodeData(); break;
    case Token::T_OPERAND_CONST_INT: stream << token.integerData(); break;
    case Token::T_OPERAND_CONST_REAL: stream << token.realData(); break;
    case Token::T_OPERAND_CONST_CHAR: stream << token.charData(); break;
    case Token::T_OPERAND_CONST_BOOL: stream << (token.booleanData() ? "true" : "false"); break;
    case Token::T_OPERAND_STACK_TOP: stream << token.stackPositionData(); break;
    case Token::T_OPERAND_STACK_BOTTOM: stream << token.stackPositionData(); break;
    case Token::T_OPERAND_HEAP_LOCATION: stream << token.heapLocationData(); break;
    case Token::T_OPERAND_PRIMARY_REGISTER: stream << "primary-register"; break;
    case Token::T_OPERAND_MANAGED_OUT_REGISTER: stream << "managed-register"; break;
    case Token::T_LABEL: stream << token.labelData(); break;
    case Token::T_NULL: stream << "null"; break;
    default: stream << "ERROR";
    }
    return stream.str();
}

void Interpreter::execute(const std::string & instruction)
{
    const Instruction & i = Lexer::tokenize(instruction);
    std::cout << (i.label.isNull() ? "" : typeString(i.label.type()) + " ")
              << typeString(i.opcode.type()) << " "
              << (i.operand1.isPointer() ? "@" : "") + typeString(i.operand1.type()) << " "
              << (i.operand2.isPointer() ? "@" : "") + typeString(i.operand2.type()) << std::endl
              << (i.label.isNull() ? "" : valueString(i.label) + " ")
              << valueString(i.opcode) << " "
              << valueString(i.operand1) << " "
              << valueString(i.operand2) << std::endl;
}
