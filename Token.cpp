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

Token::Token(const unsigned char opcode)
    : type_(T_OPCODE), isPointer_(false), opcodeData_(opcode) {}

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

bool & Token::isOptimisedLabel()
{
    return isOptimisedLabel_;
}

bool Token::isOptimisedLabel() const
{
    return isOptimisedLabel_;
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

unsigned & Token::labelLineNumberData()
{
    return labelLineNumberData_;
}

unsigned Token::labelLineNumberData() const
{
    return labelLineNumberData_;
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

Block * & Token::locationData()
{
    return locationData_;
}

Block * Token::locationData() const
{
    return locationData_;
}

void Token::setLabelData(const char * value)
{
    if (value == NULL)
    {
        strcpy(labelData_, "");
        return;
    }
    strncpy(labelData_, value, Label::length);
    type_ = T_LABEL;
    isOptimisedLabel_ = false;
}

void Token::clear()
{
    type_ = T_NULL;
    isPointer_ = false;
    isOptimisedLabel_ = false;
}

bool Token::isNull() const
{
    return type_ == T_NULL;
}
