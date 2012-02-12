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
    : type(T_NULL), isPointer(false) {}

Token::Token(const unsigned char opcode)
    : type(T_OPCODE), isPointer(false), opcodeData(opcode) {}

void Token::setLabelData(const char * value)
{
    if (value == NULL)
    {
        strcpy(labelData, "");
        return;
    }
    strncpy(labelData, value, Label::length);
    type = T_LABEL;
    isOptimisedLabel = false;
}

void Token::clear()
{
    type = T_NULL;
    isPointer = false;
    isOptimisedLabel = false;
}

bool Token::isNull() const
{
    return type == T_NULL;
}
