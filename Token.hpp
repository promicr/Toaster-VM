/*
 * Token.hpp
 *
 *  Created on: 7 Feb 2012
 *      Author: Max Foster
 */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Block.hpp"
#include "ComparisonFlagRegister.hpp"

class Integer;
class Real;
class Char;
class Boolean;

struct Token
{
    enum Type
    {
        T_OPCODE,
        T_OPERAND_CONST_INT,
        T_OPERAND_CONST_REAL,
        T_OPERAND_CONST_CHAR,
        T_OPERAND_CONST_BOOL,
        T_OPERAND_DATA_TYPE,
        T_OPERAND_STATIC_LOCATION,
        T_OPERAND_STACK_TOP,
        T_OPERAND_STACK_BOTTOM,
        T_OPERAND_STACK_NEGATIVE,
        T_OPERAND_COMPARISON_FLAG_ID,
        T_OPERAND_NIL,
        T_LABEL,
        T_NULL
    };

    Token();
    Token(const unsigned char opcode);

    void setLabelData(const char * value);
    void clear();
    bool isNull() const;

    Type type;

    union
    {
        bool isPointer, isOptimisedLabel;
    };

    union
    {
        unsigned char opcodeData;
        long integerData;
        double realData;
        char charData;
        bool booleanData;
        Block::DataType dataTypeData;
        unsigned stackPositionData, labelLineNumberData;
        CFR::ComparisonFlagId comparisonFlagData;
        char labelData[Label::length + 1];
        Block * locationData;
    };
};

#endif // TOKEN_HPP
