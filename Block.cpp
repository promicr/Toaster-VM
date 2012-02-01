/*
 * Block.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include <stdexcept>

#include "Block.hpp"
#include "Heap.hpp"

Block::Block()
{
    setUnused();
}

Block::Block(const Block & other)
{
    setUnused();
    *this = other;
}

bool Block::inUse()
{
    return inUse_;
}

Block::DataType Block::dataType()
{
    return dataType_;
}

long Block::integerData()
{
    return integerData_;
}

double Block::realData()
{
    return realData_;
}

char Block::charData()
{
    return charData_;
}

bool Block::booleanData()
{
    return booleanData_;
}

int Block::pointerAddress()
{
    return pointerData.address;
}

Heap * Block::pointerHeap()
{
    return pointerData.heap;
}

void Block::setUnused()
{
    cleanPointer();
    inUse_ = false;
}

void Block::setToInteger(const long data_)
{
    cleanPointer();
    inUse_ = true;
    dataType_ = DT_INTEGER;
    integerData_ = data_;
}

void Block::setToReal(const double data_)
{
    cleanPointer();
    inUse_ = true;
    dataType_ = DT_REAL;
    realData_ = data_;
}

void Block::setToChar(const char data_)
{
    cleanPointer();
    inUse_ = true;
    dataType_ = DT_CHAR;
    charData_ = data_;
}

void Block::setToBoolean(const bool data_)
{
    cleanPointer();
    inUse_ = true;
    dataType_ = DT_BOOLEAN;
    booleanData_ = data_;
}

void Block::setToPointer(const int address, Heap * const heap)
{
    cleanPointer();
    inUse_ = true;
    dataType_ = DT_POINTER;
    pointerData.address = address;
    pointerData.heap = heap;
    heap->incReferenceCountAt(address);
}

Block & Block::operator =(const Block & rhs)
{
    if ((dataType_ != rhs.dataType_) && inUse_)
        throw(std::runtime_error("Cannot assign data of block to a different type of block"));

    switch (rhs.dataType_)
    {
    case DT_INTEGER:
        if (inUse_) integerData_ = rhs.integerData_;
        else setToInteger(rhs.integerData_);
        break;

    case DT_REAL:
        if (inUse_) realData_ = rhs.realData_;
        else setToReal(rhs.realData_);
        break;

    case DT_CHAR:
        if (inUse_) charData_ = rhs.charData_;
        else setToChar(rhs.charData_);
        break;

    case DT_BOOLEAN:
        if (inUse_) booleanData_ = rhs.booleanData_;
        else setToBoolean(rhs.booleanData_);
        break;

    case DT_POINTER:
        setToPointer(rhs.pointerData.address, rhs.pointerData.heap);
        break;

    default: throw(std::runtime_error("Unknown type handled in assignment"));
    }

    return *this;
}

bool Block::operator ==(const Block & rhs)
{
    if (dataType_ != rhs.dataType_) return false;
    switch (dataType_)
    {
    case DT_INTEGER:
        return integerData_ == rhs.integerData_;
    case DT_REAL:
        return realData_ == rhs.realData_;
    case DT_CHAR:
        return charData_ == rhs.charData_;
    case DT_BOOLEAN:
        return booleanData_ == rhs.booleanData_;
    case DT_POINTER:
        return (pointerData.address == rhs.pointerData.address) && (pointerData.heap == rhs.pointerData.heap);

    default: throw(std::runtime_error("Unknown type handled in assignment"));
    }
}

bool Block::operator !=(const Block & rhs)
{
    return !(*this == rhs);
}

void Block::cleanPointer()
{
    if ((dataType_ == DT_POINTER) && (pointerData.heap != NULL))
        pointerData.heap->decReferenceCountAt(pointerData.address);
    pointerData.address = -1;
    pointerData.heap = NULL;
    inUse_ = false;
}
