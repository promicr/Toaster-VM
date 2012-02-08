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

//  1      2      3       4       5
const std::string Lexer::opcodeStrings[] =
{ "clr", "set", "move", "swap", "in",    //  1
  "ins", "out", "outs", "push", "pop",   //  2
  "inc", "dec", "neg",  "abs",  "add",   //  3
  "sub", "mul", "div",  "mod",  "sadd",  //  4
  "ssub","smul","sdiv", "smod", "allc",  //  5
  "pla", "fna", "atoa", "ael",  "alen",  //  6
  "cpya","cnvi","cnvr", "cnvc", "cnvb",  //  7
  "cnvt","dref","cmp",  "cmpt", "ist",   //  8
  "cpyf","not", "and",  "or",   "xor",   //  9
  "jump","je",  "jne",  "jl",   "jg",    // 10
  "jle", "jge", "call", "ret",  "extl",  // 11
  "extc", "#" };                         // 12

const short Lexer::opcodeOperandCounts[] =
{   1,     2,     2,      2,      1,     //  1
    1,     1,     1,      1,      1,     //  2
    1,     1,     1,      1,      2,     //  3
    2,     2,     2,      2,      0,     //  4
    0,     0,     0,      0,      2,     //  5
    1,     0,     1,      2,      2,     //  6
    2,     2,     2,      2,      2,     //  7
    2,     2,     2,      2,      2,     //  8
    2,     1,     2,      2,      2,     //  9
    1,     1,     1,      1,      1,     // 10
    1,     1,     1,      1,      1,     // 11
    1,     -1                            // 12
};

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
            char label[Token::labelLength + 1];
            strncpy(label, cleanString.substr(0, colonPos).c_str(), Token::labelLength);
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
    if (tokens.opcode.isNull()) throw(std::runtime_error("Lexer::tokenize: Opcode not found"));

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
    bool isNegative = false;
    for (unsigned i = stringStart; i < str.size(); ++i)
    {
        char c = str[i];
        if (!isdigit(c))
        {
            if (c == '-')
            {
                if (isNegative) throw(std::runtime_error("Lexer::getInteger: Invalid integer constant specified "
                                                         "(only one negative sign allowed)"));
                else
                {
                    isNegative = true;
                    continue;
                }
            }
            throw(std::runtime_error("Lexer::getInteger: Invalid integer constant specified "
                                     "(must only contain digits)"));
        }
        previousInteger = integer;
        integer = (integer * 10) + (int)(c - '0');

        // check for overflow
        if (integer < previousInteger)
            throw(std::runtime_error("Lexer::getInteger: Integer constant specified is too large"));
    }

    token.type() = Token::T_OPERAND_CONST_INT;
    token.integerData() = (isNegative ? -integer : integer);
}

void getReal(const std::string & str, const unsigned stringStart, Token & token)
{
    double real = 0.0, previousReal = 0.0;
    bool dotEncountered = false, isNegative = false;
    unsigned short digitsFromDot = 1;
    for (unsigned i = stringStart; i < str.size(); ++i)
    {
        char c = str[i];
        if (!isdigit(c))
        {
            if (c == '-')
            {
                if (isNegative) throw(std::runtime_error("Lexer::getReal: Invalid real constant specified "
                                                         "(only one negative sign allowed)"));
                else
                {
                    isNegative = true;
                    continue;
                }
            }
            if (c != '.')
                throw(std::runtime_error("Lexer::getReal: Invalid real constant specified "
                                         "(must only contain digits, with a single decimal point)"));
            if (dotEncountered)
                throw(std::runtime_error("Lexer::getReal: Only one decimal point can be present in a real constant"));
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
        if (real < previousReal) throw(std::runtime_error("Lexer::getReal: Real constant specified is too large"));
    }

    token.type() = Token::T_OPERAND_CONST_REAL;
    token.realData() = (isNegative? -real : real);
}

void getConstant(const std::string & str, const unsigned stringStart, Token & token)
{
    switch (str[stringStart])
    {
    case '\'':
        if ((str.size() != stringStart + 3) || (str[stringStart + 2] != '\''))
            throw(std::runtime_error("Lexer::getConstant: Invalid character constant given"));
        token.type() = Token::T_OPERAND_CONST_CHAR;
        token.charData() = str[stringStart + 1];
        break;

    case 'T':
    case 'F':
        if (str.size() != stringStart + 1)
            throw(std::runtime_error("Lexer::getConstant: Invalid boolean constant given"));
        token.type() = Token::T_OPERAND_CONST_BOOL;
        token.booleanData() = (str[stringStart] == 'T');
        break;

    default:
        if (!isdigit(str[stringStart]) && (str[stringStart] != '-'))
            throw(std::runtime_error("Lexer::getConstant: Invalid constant given"));
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
    case 'N': token.type() = Token::T_OPERAND_STACK_NEGATIVE; break;
    default: throw(std::runtime_error("Lexer::getStackLocation: Stack location not specified"));
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
        if (!isdigit(c))
            throw(std::runtime_error("Lexer::getStackLocation: Invalid stack position specified "
                                     "(must only contain digits)"));
        previousPosition = position;
        position = (position * 10) + (int)(c - '0');

        // check for overflow
        if (position < previousPosition)
            throw(std::runtime_error("Lexer::getStackLocation: Stack position specified is too large"));
    }

    token.stackPositionData() = position;
}

void getRegister(const std::string & str, const unsigned stringStart, Token & token)
{
    switch (str[stringStart])
    {
    case 'P': token.type() = Token::T_OPERAND_PRIMARY_REGISTER; break;
    case 'M': token.type() = Token::T_OPERAND_MANAGED_OUT_REGISTER; break;
    default: throw(std::runtime_error("Lexer::getRegister: Register not specified"));
    }
}

void getHeapLocation(const std::string & str, const unsigned stringStart, Token & token)
{
    unsigned location = 0, previousLocation = 0;
    for (unsigned i = stringStart; i < str.size(); ++i)
    {
        char c = str[i];
        if (!isdigit(c))
            throw(std::runtime_error("Lexer::getHeapLocation: Invalid heap location specified "
                                     "(must only contain digits)"));
        previousLocation = location;
        location = (location * 10) + (int)(c - '0');

        // check for overflow
        if (location < previousLocation)
            throw(std::runtime_error("Lexer::getHeapLocation: Heap location specified is too large"));
    }

    token.type() = Token::T_OPERAND_HEAP_LOCATION;
    token.heapLocationData() = location;
}

void getLabel(const std::string & str, const unsigned stringStart, Token & token)
{
    if (stringStart + Token::labelLength < str.size())
        throw(std::runtime_error("Lexer::getLabel: Label given is too long"));

    token.type() = Token::T_LABEL;
    unsigned i = 0;
    for (i = 0; (i < Token::labelLength) && (stringStart + i < str.size()); ++i)
        token.labelData()[i] = str[stringStart + i];
    token.labelData()[i] = '\0';
}

void getLocation(const std::string & str, const unsigned stringStart, Token & token)
{
    switch (str[stringStart])
    {
    case 'S': getStackLocation(str, stringStart + 1, token); break;
    case 'R': getRegister(str, stringStart + 1, token); break;
    default:
        if (isdigit(str[stringStart])) getHeapLocation(str, stringStart, token);
        else if (isalpha(str[stringStart])) getLabel(str, stringStart, token);
        else throw(std::runtime_error("Lexer::getLocation: Invalid keyword given"));
        break;
    }
}

void getPointer(const std::string & str, const unsigned stringStart, Token & token)
{
    getLocation(str, stringStart, token);
    token.isPointer() = true;
}

void getDataType(const std::string & str, const unsigned stringStart, Token & token)
{
    token.type() = Token::T_OPERAND_DATA_TYPE;
    switch (str[stringStart])
    {
    case 'i': token.dataTypeData() = Block::DT_INTEGER; break;
    case 'r': token.dataTypeData() = Block::DT_REAL; break;
    case 'c': token.dataTypeData() = Block::DT_CHAR; break;
    case 'b': token.dataTypeData() = Block::DT_BOOLEAN; break;
    case 'p': token.dataTypeData() = Block::DT_POINTER; break;
    default: throw(std::runtime_error("Lexer::getDataType: Invalid data type given (expected i, r, c, b or p)"));
    }
}

inline void throwCFRIdError()
{
    throw(std::runtime_error("Lexer::getComparisonFlagId: Invalid comparison flag given "
                             "(expected eq, ne, lt, gt, le or ge)"));
}

void getComparisonFlagId(const std::string & str, const unsigned stringStart, Token & token)
{
    if (str.size() < 3) throwCFRIdError();

    token.type() = Token::T_OPERAND_COMPARISON_FLAG_ID;
    char nextChar = str[stringStart + 1];
    switch (str[stringStart])
    {
    case 'e':
        if (nextChar == 'q') token.comparisonFlagData() = CFR::F_EQUAL;
        else throwCFRIdError();
        break;

    case 'n':
        if (nextChar == 'e') token.comparisonFlagData() = CFR::F_NOT_EQUAL;
        else throwCFRIdError();
        break;

    case 'l':
        if (nextChar == 't') token.comparisonFlagData() = CFR::F_LESS;
        else if (nextChar == 'e') token.comparisonFlagData() = CFR::F_LESS_EQUAL;
        else throwCFRIdError();
        break;

    case 'g':
        if (nextChar == 't') token.comparisonFlagData() = CFR::F_GREATER;
        else if (nextChar == 'e') token.comparisonFlagData() = CFR::F_GREATER_EQUAL;
        else throwCFRIdError();
        break;

    default: throwCFRIdError();
    }
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
    case '$': getDataType(str, 1, returnToken); break;
    case '?': getComparisonFlagId(str, 1, returnToken); break;
    case 'n':
        if ((str.size() == 3) && (str[1] == 'i') && (str[2] == 'l'))
        {
            returnToken.type() = Token::T_OPERAND_NIL;
            break;
        } // else fall into default
    default:  getLocation(str, 0, returnToken);
    }
    return returnToken;
}