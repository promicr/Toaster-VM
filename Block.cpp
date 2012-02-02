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
    pointerData.heap = NULL;
    setUnused();
}

Block::Block(const Block & other)
{
    pointerData.heap = NULL;
    if (other.dataType_ != DATA_TYPE_COUNT)
    {
        setUnused();
        *this = other;
    }
}

Block::Block(const int address, Heap & heap)
{
    pointerData.heap = NULL;
    setToPointer(address, heap);
}

Block::Block(const DataType dataType)
{
    pointerData.heap = NULL;
    setTo(dataType);
}

Block::~Block()
{
    nullifyPointer();
}

bool Block::inUse() const
{
    return inUse_;
}

Block::DataType Block::dataType() const
{
    return dataType_;
}

long Block::integerData() const
{
    return integerData_;
}

long & Block::integerData()
{
    return integerData_;
}

double Block::realData() const
{
    return realData_;
}

double & Block::realData()
{
    return realData_;
}

char Block::charData() const
{
    return charData_;
}

char & Block::charData()
{
    return charData_;
}

bool Block::booleanData() const
{
    return booleanData_;
}

bool & Block::booleanData()
{
    return booleanData_;
}

int Block::pointerAddress() const
{
    return pointerData.address;
}

int & Block::pointerAddress()
{
    return pointerData.address;
}

Heap & Block::pointerHeap() const
{
    return *pointerData.heap;
}

bool Block::pointerIsNull() const
{
    return (pointerData.address < 0) || (pointerData.heap == NULL);
}

void Block::setUnused(const bool decReference)
{
    nullifyPointer(decReference);
    dataType_ = DT_INTEGER;
    inUse_ = false;
}

void Block::setToInteger(const long data_)
{
    nullifyPointer();
    inUse_ = true;
    dataType_ = DT_INTEGER;
    integerData_ = data_;
}

void Block::setToReal(const double data_)
{
    nullifyPointer();
    inUse_ = true;
    dataType_ = DT_REAL;
    realData_ = data_;
}

void Block::setToChar(const char data_)
{
    nullifyPointer();
    inUse_ = true;
    dataType_ = DT_CHAR;
    charData_ = data_;
}

void Block::setToBoolean(const bool data_)
{
    nullifyPointer();
    inUse_ = true;
    dataType_ = DT_BOOLEAN;
    booleanData_ = data_;
}

void Block::setToPointer(const int address, Heap & heap)
{
    nullifyPointer();
    inUse_ = true;
    dataType_ = DT_POINTER;
    pointerData.address = address;
    pointerData.heap = &heap;
    heap.incReferenceCountAt(address);
}

void Block::setToPointer()
{
    nullifyPointer();
    inUse_ = true;
    dataType_ = DT_POINTER;
}

void Block::setTo(DataType dataType)
{
    switch (dataType)
    {
    case DT_INTEGER: setToInteger(); break;
    case DT_REAL:    setToReal(); break;
    case DT_CHAR:    setToChar(); break;
    case DT_BOOLEAN: setToBoolean(); break;
    case DT_POINTER: setToPointer(); break;
    default: throw(std::runtime_error("Unknown block data type"));
    }
}

Block & Block::operator =(const Block & rhs)
{    
    if ((dataType_ != rhs.dataType_) && inUse_)
        throw(std::runtime_error("Cannot assign data of block to a different type of block"));

    inUse_ = rhs.inUse_;
    if (!inUse_) return *this;

    switch (rhs.dataType_)
    {
    case DT_INTEGER: integerData_ = rhs.integerData_; break;
    case DT_REAL:    realData_ = rhs.realData_; break;
    case DT_CHAR:    charData_ = rhs.charData_; break;
    case DT_BOOLEAN: booleanData_ = rhs.booleanData_; break;
    case DT_POINTER: setToPointer(rhs.pointerData.address, *rhs.pointerData.heap); break;
    default: throw(std::runtime_error("Unknown type handled in assignment"));
    }

    return *this;
}

bool Block::operator ==(const Block & rhs) const
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

bool Block::operator !=(const Block & rhs) const
{
    return !(*this == rhs);
}

void Block::nullifyPointer(const bool decReference)
{
    if ((dataType_ == DT_POINTER) && (pointerData.heap != NULL) && decReference)
        pointerData.heap->decReferenceCountAt(pointerData.address);
    pointerData.address = -1;
    pointerData.heap = NULL;
}

std::ostream & operator <<(std::ostream & stream, const Block & block)
{
    switch (block.dataType())
    {
    case Block::DT_INTEGER: stream << block.integerData(); break;
    case Block::DT_REAL:    stream << block.realData(); break;
    case Block::DT_CHAR:    stream << block.charData(); break;
    case Block::DT_BOOLEAN: stream << block.booleanData(); break;
    case Block::DT_POINTER: stream << block.pointerAddress(); break;
    default: break;
    }

    return stream;
}
