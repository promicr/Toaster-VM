/*
 * Block.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include "Block.hpp"

Block::Block()
{
    setToNull();
}

void Block::setToNull()
{
    dataType = DT_NULL;
}

void Block::setToInteger(const long data_)
{
    dataType = DT_INTEGER;
    data.integerData = data_;
}

void Block::setToReal(const double data_)
{
    dataType = DT_REAL;
    data.realData = data_;
}

void Block::setToChar(const char data_)
{
    dataType = DT_CHAR;
    data.charData = data_;
}

void Block::setToBoolean(const bool data_)
{
    dataType = DT_BOOLEAN;
    data.booleanData = data_;
}

void Block::setToPointer(const int address, const bool inManagedHeap, const DataType dataType_, const int elementCount)
{
    dataType = DT_POINTER;
    data.pointerData.address.value = address;
    data.pointerData.address.inManagedHeap = inManagedHeap;
    data.pointerData.dataType = dataType_;
    data.pointerData.elementCount = elementCount;
}