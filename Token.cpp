/*
 * Token.cpp
 *
 *  Created on: 7 Feb 2012
 *      Author: Max Foster
 */

#include <cstring>
#include <stdexcept>

#include "Token.hpp"
#include "TypeWrappers.hpp"

Token::Token()
    : type_(T_NULL), isPointer_(false) {}

Token::Token(const unsigned char value)
    : type_(T_OPCODE), isPointer_(false), opcodeData_(value) {}

Token::Token(const Integer value)
    : type_(T_OPERAND_CONST_INT), isPointer_(false), integerData_(value) {}

Token::Token(const Real value)
    : type_(T_OPERAND_CONST_REAL), isPointer_(false), realData_(value) {}

Token::Token(const Char value)
    : type_(T_OPERAND_CONST_CHAR), isPointer_(false), charData_(value) {}

Token::Token(const Boolean value)
    : type_(T_OPERAND_CONST_BOOL), isPointer_(false), booleanData_(value) {}

Token::Token(const Block::DataType value)
    : type_(T_OPERAND_DATA_TYPE), isPointer_(false), dataTypeData_(value) {}

Token::Token(const unsigned value, const bool ftos, const bool isPointer)
    : type_(ftos ? T_OPERAND_STACK_TOP : T_OPERAND_STACK_BOTTOM), isPointer_(isPointer), stackPositionData_(value) {}

Token::Token(const unsigned value, const bool isPointer)
    : type_(T_OPERAND_HEAP_LOCATION), isPointer_(isPointer), heapLocationData_(value) {}

Token::Token(const RegisterId registerId, const bool isPointer)
    : isPointer_(isPointer)
{
    switch (registerId)
    {
    case R_PRIMARY:     type_ = T_OPERAND_PRIMARY_REGISTER; break;
    case R_MANAGED_OUT: type_ = T_OPERAND_MANAGED_OUT_REGISTER; break;
    default: throw(std::runtime_error("Token::Token: Unknown register ID given"));
    }
}

Token::Token(const CFR::ComparisonFlagId value)
    : type_(T_OPERAND_COMPARISON_FLAG_ID), isPointer_(false), comparisonFlagData_(value) {}

Token::Token(const char * value)
    : type_(T_LABEL), isPointer_(false)
{
    if (value == NULL)
    {
        strcpy(labelData_, "");
        return;
    }
    strncpy(labelData_, value, 12);
}

Token::Type & Token::type()
{
    return type_;
}

Token::Type Token::type() const
{
    return type_;
}

bool & Token::isPointer()
{
    return isPointer_;
}

bool Token::isPointer() const
{
    return isPointer_;
}

unsigned char & Token::opcodeData()
{
    return opcodeData_;
}

unsigned char Token::opcodeData() const
{
    return opcodeData_;
}

long & Token::integerData()
{
    return integerData_;
}

long Token::integerData() const
{
    return integerData_;
}

double & Token::realData()
{
    return realData_;
}

double Token::realData() const
{
    return realData_;
}

char & Token::charData()
{
    return charData_;
}

char Token::charData() const
{
    return charData_;
}

bool & Token::booleanData()
{
    return booleanData_;
}

bool Token::booleanData() const
{
    return booleanData_;
}

Block::DataType & Token::dataTypeData()
{
    return dataTypeData_;
}

Block::DataType Token::dataTypeData() const
{
    return dataTypeData_;
}

unsigned & Token::stackPositionData()
{
    return stackPositionData_;
}

unsigned Token::stackPositionData() const
{
    return stackPositionData_;
}

unsigned & Token::heapLocationData()
{
    return heapLocationData_;
}

unsigned Token::heapLocationData() const
{
    return heapLocationData_;
}

CFR::ComparisonFlagId & Token::comparisonFlagData()
{
    return comparisonFlagData_;
}

CFR::ComparisonFlagId Token::comparisonFlagData() const
{
    return comparisonFlagData_;
}

char * Token::labelData()
{
    return labelData_;
}

const char * Token::labelData() const
{
    return labelData_;
}

void Token::setLabelData(const char * value)
{
    if (value == NULL)
    {
        strcpy(labelData_, "");
        return;
    }
    strncpy(labelData_, value, 12);
    type_ = T_LABEL;
}

void Token::clear()
{
    type_ = T_NULL;
    isPointer_ = false;
}

bool Token::isNull() const
{
    return type_ == T_NULL;
}
