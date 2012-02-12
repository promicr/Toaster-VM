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
        T_OPERAND_STACK_TOP,
        T_OPERAND_STACK_BOTTOM,
        T_OPERAND_STACK_NEGATIVE, // to get items from stack under stack frame pointer, i.e. arguments to function
        T_OPERAND_HEAP_LOCATION,
        T_OPERAND_PRIMARY_REGISTER,
        T_OPERAND_MANAGED_OUT_REGISTER,
        T_OPERAND_COMPARISON_FLAG_ID,
        T_OPERAND_NIL,
        T_LABEL,
        T_NULL
    };

    enum RegisterId
    {
        R_PRIMARY,
        R_MANAGED_OUT
    };

    Token();

    Token(unsigned char value);
    Token(Integer value);
    Token(Real value);
    Token(Char value);
    Token(Boolean value);
    Token(Block::DataType value);
    Token(unsigned value, bool fromTopOfStack, bool isPointer);
    Token(unsigned value, bool isPointer);
    Token(RegisterId registerId, bool isPointer);
    Token(CFR::ComparisonFlagId value);
    Token(const char * value);

    Type & type();
    Type type() const;

    bool & isPointer();
    bool isPointer() const;
    bool & isOptimisedLabel();
    bool isOptimisedLabel() const;
    bool & isOptimisedLocation();
    bool isOptimisedLocation() const;

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
    unsigned & heapLocationData();
    unsigned heapLocationData() const;
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

    bool isPointer_;
    union
    {
        bool isOptimisedLabel_, isOptimisedLocation_;
    };

    union
    {
        unsigned char opcodeData_;
        long integerData_;
        double realData_;
        char charData_;
        bool booleanData_;
        Block::DataType dataTypeData_;
        unsigned stackPositionData_, heapLocationData_, labelLineNumberData_;
        CFR::ComparisonFlagId comparisonFlagData_;
        char labelData_[Label::length + 1];
        Block * locationData_;
    };
};

#endif // TOKEN_HPP
