/*
 * Lexer.cpp
 *
 *  Created on: 7 Feb 2012
 *      Author: Max Foster
 */

#include <stdexcept>
#include <cstring>
#include <cmath>

#include "Lexer.hpp"
#include "Instruction.hpp"

const std::string Lexer::opcodeStrings[] =
{ "clr", "set", "move", "swap", "in", "ins", "out", "outs", "push", "pop", "inc", "dec", "neg", "abs", "add", "sub",
  "mul", "div", "mod", "sadd", "ssub", "smul", "sdiv", "allc", "pla", "fna" , "atoa", "ael", "alen", "cnvi", "cnvr",
  "cnvc", "cnvb", "cnvt", "dref", "cmp", "cmpt", "ist", "cpyf", "not", "and", "or", "xor", "jump", "je", "jne", "jl",
  "jg", "jle", "jge", "call", "ret", "extl", "extc", "#" };

int Lexer::getOpcodeId(const std::string & opcode)
{
    int i = 0;
    while (true)
    {
        if (opcodeStrings[i] == "#") return -1;
        if (opcodeStrings[i] == opcode) return i;
        ++i;
    }
}

inline std::string removeWhitespace(const std::string & str)
{
    int start = 0, end = str.size() - 1;
    while (isspace(str[start])) ++start;
    while (isspace(str[end])) --end;
    return str.substr(start, (end - start) + 1);
}

const Instruction & Lexer::tokenize(const std::string & instruction)
{
    static Instruction tokens;

    tokens.clear();
    if (instruction.size() == 0) return tokens;

    std::string cleanString(removeWhitespace(instruction));

    {
        size_t colonPos = cleanString.find_first_of(':');
        if (colonPos != std::string::npos)
        {
            char label[13];
            strncpy(label, cleanString.substr(0, colonPos).c_str(), 12);
            if (!isalpha(label[0])) throw(std::runtime_error("Lexer::tokenize: Labels must start with a letter"));
            tokens.label.setLabelData(label);
            if (colonPos == cleanString.size() - 1) return tokens;

            cleanString = removeWhitespace(cleanString.substr(colonPos + 1, cleanString.size() - colonPos));
            if (cleanString.find_first_of(':') != std::string::npos)
                throw(std::runtime_error("Lexer::tokenize: Only one label per line allowed"));
        }
    }

    size_t spacePos = cleanString.find_first_of(' ');
    if (spacePos == std::string::npos)
    {
        tokens.opcode = getOpcodeTokenFrom(cleanString);
        return tokens;
    }
    tokens.opcode = getOpcodeTokenFrom(cleanString.substr(0, spacePos));

    cleanString = removeWhitespace(cleanString.substr(spacePos, cleanString.size() - spacePos));
    spacePos = cleanString.find_first_of(' ');
    if (spacePos == std::string::npos)
    {
        tokens.operand1 = getOperandTokenFrom(cleanString);
        return tokens;
    }
    tokens.operand1 = getOperandTokenFrom(cleanString.substr(0, spacePos));

    cleanString = removeWhitespace(cleanString.substr(spacePos, cleanString.size() - spacePos));
    spacePos = cleanString.find_first_of(' ');
    if (spacePos == std::string::npos)
    {
        tokens.operand2 = getOperandTokenFrom(cleanString);
        return tokens;
    }
    tokens.operand2 = getOperandTokenFrom(cleanString.substr(0, spacePos));

    return tokens;
}

Token Lexer::getOpcodeTokenFrom(const std::string & str)
{
    int opcodeId = getOpcodeId(str);
    if (opcodeId < 0) return Token();
    return Token(static_cast<unsigned char>(opcodeId));
}

void getInteger(const std::string & str, const unsigned stringStart, Token & token)
{
    long integer = 0, previousInteger = 0;
    for (unsigned i = stringStart; i < str.size(); ++i)
    {
        char c = str[i];
        if (!isdigit(c)) throw(std::runtime_error("Invalid integer constant specified (must only contain digits)"));
        previousInteger = integer;
        integer = (integer * 10) + (int)(c - '0');

        // check for overflow
        if (integer < previousInteger) throw(std::runtime_error("Integer constant specified is too large"));
    }

    token.type() = Token::T_OPERAND_CONST_INT;
    token.integerData() = integer;
}

void getReal(const std::string & str, const unsigned stringStart, Token & token)
{
    double real = 0.0, previousReal = 0.0;
    bool dotEncountered = false;
    unsigned short digitsFromDot = 1;
    for (unsigned i = stringStart; i < str.size(); ++i)
    {
        char c = str[i];
        if (!isdigit(c))
        {
            if (c != '.')
                throw(std::runtime_error(
                        "Invalid real constant specified (must only contain digits, with a single decimal point)"));
            if (dotEncountered) throw(std::runtime_error("Only one decimal point can be present in a real constant"));
            dotEncountered = true;
            continue;
        }
        previousReal = real;
        if (!dotEncountered) real = (real * 10) + (int)(c - '0');
        else
        {
            real += (double)(c - '0') / pow(10.0, digitsFromDot);
            ++digitsFromDot;
        }

        // check for overflow
        if (real < previousReal) throw(std::runtime_error("Real constant specified is too large"));
    }

    token.type() = Token::T_OPERAND_CONST_REAL;
    token.realData() = real;
}

void getConstant(const std::string & str, const unsigned stringStart, Token & token)
{
    switch (str[stringStart])
    {
    case '\'':
        if ((str.size() != stringStart + 3) || (str[stringStart + 2] != '\''))
            throw(std::runtime_error("Invalid character constant given"));
        token.type() = Token::T_OPERAND_CONST_CHAR;
        token.charData() = str[stringStart + 1];
        break;

    case 'T':
    case 'F':
        if (str.size() != stringStart + 1) throw(std::runtime_error("Invalid boolean constant given"));
        token.type() = Token::T_OPERAND_CONST_BOOL;
        token.booleanData() = (str[stringStart] == 'T');
        break;

    default:
        if (!isdigit(str[stringStart])) throw(std::runtime_error("Invalid constant given"));
        if (str.find('.') != std::string::npos) getReal(str, stringStart, token);
        else getInteger(str, stringStart, token);
    }
}

void getStackLocation(const std::string & str, const unsigned stringStart, Token & token)
{
    switch (str[stringStart])
    {
    case 'T': token.type() = Token::T_OPERAND_STACK_TOP; break;
    case 'B': token.type() = Token::T_OPERAND_STACK_BOTTOM; break;
    default: throw(std::runtime_error("Stack location not specified"));
    }

    if (str.size() == stringStart + 1)
    {
        token.stackPositionData() = 0;
        return;
    }

    unsigned position = 0, previousPosition = 0;
    for (unsigned i = stringStart + 1; i < str.size(); ++i)
    {
        char c = str[i];
        if (!isdigit(c)) throw(std::runtime_error("Invalid stack position specified (must only contain digits)"));
        previousPosition = position;
        position = (position * 10) + (int)(c - '0');

        // check for overflow
        if (position < previousPosition) throw(std::runtime_error("Stack position specified is too large"));
    }

    token.stackPositionData() = position;
}

void getRegister(const std::string & str, const unsigned stringStart, Token & token)
{
    switch (str[stringStart])
    {
    case 'P': token.type() = Token::T_OPERAND_PRIMARY_REGISTER; break;
    case 'M': token.type() = Token::T_OPERAND_MANAGED_OUT_REGISTER; break;
    default: throw(std::runtime_error("Register not specified"));
    }
}

void getHeapLocation(const std::string & str, const unsigned stringStart, Token & token)
{
    unsigned location = 0, previousLocation = 0;
    for (unsigned i = stringStart; i < str.size(); ++i)
    {
        char c = str[i];
        if (!isdigit(c)) throw(std::runtime_error("Invalid heap location specified (must only contain digits)"));
        previousLocation = location;
        location = (location * 10) + (int)(c - '0');

        // check for overflow
        if (location < previousLocation) throw(std::runtime_error("Heap location specified is too large"));
    }

    token.type() = Token::T_OPERAND_HEAP_LOCATION;
    token.heapLocationData() = location;
}

void getLocation(const std::string & str, const unsigned stringStart, Token & token)
{
    switch (str[stringStart])
    {
    case 'S': getStackLocation(str, stringStart + 1, token); break;
    case 'R': getRegister(str, stringStart + 1, token); break;
    default:  if (isdigit(str[stringStart])) getHeapLocation(str, stringStart, token); break;
    }
}

void getPointer(const std::string & str, const unsigned stringStart, Token & token)
{
    getLocation(str, stringStart, token);
    token.isPointer() = true;
}

Token Lexer::getOperandTokenFrom(const std::string & str)
{
    Token returnToken;
    if (str.size() < 2)
    {
        if (isdigit(str[0])) getHeapLocation(str, 0, returnToken);
        return returnToken;
    }
    switch (str[0])
    {
    case '#': getConstant(str, 1, returnToken); break;
    case '@': getPointer(str, 1, returnToken); break;
    default:  getLocation(str, 0, returnToken);
    }
    return returnToken;
}
