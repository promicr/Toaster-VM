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

class Token
{
public:
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

    Type & type();
    Type type() const;

    bool & isPointer();
    bool isPointer() const;
    bool & isOptimisedLabel();
    bool isOptimisedLabel() const;

    unsigned char & opcodeData();
    unsigned char opcodeData() const;
    long & integerData();
    long integerData() const;
    double & realData();
    double realData() const;
    char & charData();
    char charData() const;
    bool & booleanData();
    bool booleanData() const;
    Block::DataType & dataTypeData();
    Block::DataType dataTypeData() const;
    unsigned & stackPositionData();
    unsigned stackPositionData() const;
    unsigned & labelLineNumberData();
    unsigned labelLineNumberData() const;
    CFR::ComparisonFlagId & comparisonFlagData();
    CFR::ComparisonFlagId comparisonFlagData() const;
    char * labelData();
    const char * labelData() const;
    Block * & locationData();
    Block * locationData() const;

    void setLabelData(const char * value);
    void clear();
    bool isNull() const;

private:
    Type type_;

    union
    {
        bool isPointer_, isOptimisedLabel_;
    };

    union
    {
        unsigned char opcodeData_;
        long integerData_;
        double realData_;
        char charData_;
        bool booleanData_;
        Block::DataType dataTypeData_;
        unsigned stackPositionData_, labelLineNumberData_;
        CFR::ComparisonFlagId comparisonFlagData_;
        char labelData_[Label::length + 1];
        Block * locationData_;
    };
};

#endif // TOKEN_HPP
