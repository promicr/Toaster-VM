/*
 * Block.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include <stdexcept>
#include <cmath>

#include "Block.hpp"
#include "ManagedHeap.hpp"

Block::Block()
{
    pointerData.heap = NULL;
}

Block::Block(const Block & other)
{
    pointerData.heap = NULL;
    if (other.dataType_ != DATA_TYPE_COUNT) *this = other;
}

Block::Block(const Integer value)
{
    dataType_ = DT_INTEGER;
    integerData_ = value;
}

Block::Block(const Real value)
{
    dataType_ = DT_REAL;
    realData_ = value;
}

Block::Block(const Char value)
{
    dataType_ = DT_CHAR;
    charData_ = value;
}

Block::Block(const Boolean value)
{
    dataType_ = DT_BOOLEAN;
    booleanData_ = value;
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
    nullifyPointerData();
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

Heap * Block::pointerHeap() const
{
    if (dataType_ != DT_POINTER) return NULL;
    return pointerData.heap;
}

bool Block::pointerIsNull() const
{
    if (dataType_ != DT_POINTER) return true;
    return (pointerData.address < 0) || (pointerData.heap == NULL);
}

unsigned Block::pointerArrayLength() const
{
    if (pointerIsNull()) return 0;
    ManagedHeap * managedHeap = dynamic_cast<ManagedHeap*>(pointerData.heap);
    if (managedHeap == NULL) return 0;
    return managedHeap->arrayLengthAt(pointerData.address);
}

Block * Block::pointerArrayElementAt(const unsigned index) const
{
    if (pointerIsNull() || (index >= pointerArrayLength())) return NULL;
    return &pointerData.heap->blockAt(pointerData.address + index);
}

Block * Block::pointerDataPointedTo() const
{
    if (pointerIsNull()) return NULL;
    return &pointerData.heap->blockAt(pointerData.address);
}

void Block::nullifyPointerData(const bool decReference)
{
    if ((dataType_ == DT_POINTER) && (pointerData.heap != NULL) && decReference)
        pointerData.heap->decReferenceCountAt(pointerData.address);
    pointerData.address = -1;
    pointerData.heap = NULL;
}

void Block::setToInteger(const long data_)
{
    nullifyPointerData();
    dataType_ = DT_INTEGER;
    integerData_ = data_;
}

void Block::setToReal(const double data_)
{
    nullifyPointerData();
    dataType_ = DT_REAL;
    realData_ = data_;
}

void Block::setToChar(const char data_)
{
    nullifyPointerData();
    dataType_ = DT_CHAR;
    charData_ = data_;
}

void Block::setToBoolean(const bool data_)
{
    nullifyPointerData();
    dataType_ = DT_BOOLEAN;
    booleanData_ = data_;
}

void Block::setToPointer(const int address, Heap & heap)
{
    nullifyPointerData();
    dataType_ = DT_POINTER;
    pointerData.address = address;
    pointerData.heap = &heap;
    heap.incReferenceCountAt(address);
}

void Block::setToPointer()
{
    nullifyPointerData();
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
    default: throw(std::runtime_error("Block::setTo: Unknown block data type"));
    }
}

void Block::clear()
{
    setTo(dataType_);
}

Block & Block::operator =(const Block & rhs)
{
    nullifyPointerData();
    dataType_ = rhs.dataType_;
    if (dataType_ == DT_POINTER) setToPointer(rhs.pointerData.address, *rhs.pointerData.heap);
    else pointerData = rhs.pointerData; // because pointerData takes up most space in the union
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
        return fabs(realData_ - rhs.realData_) < 0.00001;
    case DT_CHAR:
        return charData_ == rhs.charData_;
    case DT_BOOLEAN:
        return booleanData_ == rhs.booleanData_;
    case DT_POINTER:
        return (pointerData.address == rhs.pointerData.address) && (pointerData.heap == rhs.pointerData.heap);

    default:
        std::cout << "Unknown type handled in assignment" << std::endl;
        return false;
    }
}

bool Block::operator !=(const Block & rhs) const
{
    return !(*this == rhs);
}

std::ostream & operator <<(std::ostream & stream, const Block & block)
{
    switch (block.dataType())
    {
    case Block::DT_INTEGER: stream << block.integerData(); break;
    case Block::DT_REAL:    stream << block.realData(); break;
    case Block::DT_CHAR:    stream << block.charData(); break;
    case Block::DT_BOOLEAN: stream << (block.booleanData() ? "true" : "false"); break;
    case Block::DT_POINTER: stream << block.pointerAddress(); break;
    default: break;
    }

    return stream;
}
