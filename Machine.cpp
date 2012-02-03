/*
 * Machine.cpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#include <cstdio>

#include "Machine.hpp"

Machine::Machine(const unsigned stackSize, const unsigned unmanagedHeapSize, const unsigned managedHeapSize)
    : stack_(stackSize), unmanagedHeap_(unmanagedHeapSize), managedHeap_(managedHeapSize) {}

void Machine::clear(const locationId location)
{
    Block * block = getBlockFrom(location);
    if (block != NULL) clear(block);
}

void Machine::clear(Block & location, const bool locationIsPointer)
{
    Block * block = locationIsPointer ? getBlockFrom(location) : &location;
    if (block != NULL) clear(block);
}

void Machine::clear(Block * locationBlock)
{
    locationBlock->clear();
}

////////////////////////////////////////////////////////////

void Machine::set(const Block & value, const locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL) set(value, block);
}

void Machine::set(const Block & value, Block & destination, const bool destIsPointer)
{
    Block * block = destIsPointer ? getBlockFrom(destination) : &destination;
    if (block != NULL) set(value, block);
}

void Machine::set(const Block & value, Block * destBlock)
{
    *destBlock = value;
}

////////////////////////////////////////////////////////////

void Machine::move(const locationId source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    move(sourceBlock, destBlock);
}

void Machine::move(const locationId source, Block & destination, const bool destIsPointer)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    move(sourceBlock, destBlock);
}

void Machine::move(const Block & source, const bool sourceIsPointer, const locationId destination)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    move(sourceBlock, destBlock);
}

void Machine::move(const Block & source, const bool sourceIsPointer, Block & destination, const bool destIsPointer)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;

    move(sourceBlock, destBlock);
}

void Machine::move(const Block * sourceBlock, Block * destBlock)
{
    *destBlock = *sourceBlock;
}

////////////////////////////////////////////////////////////

void Machine::read(const locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL)
    {
        char c;
        if (scanf("%c", &c) != 0) block->setToChar(c);
    }
}

void Machine::read(Block & destination, const bool destIsPointer)
{
    Block * block = destIsPointer ? getBlockFrom(destination) : &destination;
    if (block != NULL) read(block);
}

void Machine::read(Block * destBlock)
{
    char c;
    if (scanf("%c", &c) != 0) destBlock->setToChar(c);
}

////////////////////////////////////////////////////////////

void Machine::write(const locationId source)
{
    const Block * block = getBlockFrom(source);
    if (block != NULL) write(block);
}

void Machine::write(const Block & source, const bool sourceIsPointer)
{
    const Block * block = sourceIsPointer ? getBlockFrom(source) : &source;
    if (block != NULL) write(block);
}

void Machine::write(const Block * sourceBlock)
{
    std::cout << *sourceBlock << std::endl;
}

////////////////////////////////////////////////////////////

void Machine::push(const locationId source)
{
    const Block * block = getBlockFrom(source);
    if (block != NULL) push(block);
}

void Machine::push(const Block & source, const bool sourceIsPointer)
{
    const Block * block = sourceIsPointer ? getBlockFrom(source) : &source;
    if (block != NULL) push(block);
}

void Machine::push(const Block * sourceBlock)
{
    stack_.push(*sourceBlock);
}

////////////////////////////////////////////////////////////

void Machine::pop(const locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL) pop(block);
}

void Machine::pop(Block & destination, const bool destIsPointer)
{
    Block * block = destIsPointer ? getBlockFrom(destination) : &destination;
    if (block != NULL) pop(block);
}

void Machine::pop(Block * destBlock)
{
    *destBlock = stack_.peek();
    stack_.pop();
}

////////////////////////////////////////////////////////////

void Machine::increment(const locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL) increment(block);
}

void Machine::increment(Block & destination, const bool destIsPointer)
{
    Block * block = destIsPointer ? getBlockFrom(destination) : &destination;
    if (block != NULL) increment(block);
}

void Machine::increment(Block * destBlock)
{
    switch (destBlock->dataType())
    {
    case Block::DT_INTEGER: ++destBlock->integerData(); break;
    case Block::DT_REAL:    ++destBlock->realData(); break;
    case Block::DT_CHAR:    ++destBlock->charData(); break;
    case Block::DT_POINTER:
        if (!destBlock->pointerIsNull() && (destBlock->pointerAddress() < (int)destBlock->pointerHeap()->size()))
            ++destBlock->pointerAddress();
        break;

    default: break;
    }
}

////////////////////////////////////////////////////////////

void Machine::decrement(const locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL) decrement(block);
}

void Machine::decrement(Block & destination, const bool destIsPointer)
{
    Block * block = destIsPointer ? getBlockFrom(destination) : &destination;
    if (block != NULL) decrement(block);
}

void Machine::decrement(Block * destBlock)
{
    switch (destBlock->dataType())
    {
    case Block::DT_INTEGER: --destBlock->integerData(); break;
    case Block::DT_REAL:    --destBlock->realData(); break;
    case Block::DT_CHAR:    --destBlock->charData(); break;
    case Block::DT_POINTER:
        if (!destBlock->pointerIsNull() && (destBlock->pointerAddress() > 0))
            --destBlock->pointerAddress();
        break;

    default: break;
    }
}

////////////////////////////////////////////////////////////

void Machine::add(const locationId source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const locationId source, Block & destination, const bool destIsPointer)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const Block & source, const bool sourceIsPointer, const locationId destination)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const Block & source, const bool sourceIsPointer, Block & destination, const bool destIsPointer)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const Block * sourceBlock, Block * destBlock)
{
    if (sourceBlock->dataType() != destBlock->dataType()) return;

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() += sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() += sourceBlock->realData();
        break;

    default: break;
    }
}

////////////////////////////////////////////////////////////

void Machine::subtract(const locationId source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const locationId source, Block & destination, const bool destIsPointer)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const Block & source, const bool sourceIsPointer, const locationId destination)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const Block & source, const bool sourceIsPointer, Block & destination, const bool destIsPointer)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const Block * sourceBlock, Block * destBlock)
{
    if (sourceBlock->dataType() != destBlock->dataType()) return;

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() -= sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() -= sourceBlock->realData();
        break;

    default: break;
    }
}

////////////////////////////////////////////////////////////

void Machine::multiply(const locationId source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const locationId source, Block & destination, const bool destIsPointer)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const Block & source, const bool sourceIsPointer, const locationId destination)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const Block & source, const bool sourceIsPointer, Block & destination, const bool destIsPointer)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const Block * sourceBlock, Block * destBlock)
{
    if (sourceBlock->dataType() != destBlock->dataType()) return;

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() *= sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() *= sourceBlock->realData();
        break;

    default: break;
    }
}

////////////////////////////////////////////////////////////

void Machine::divide(const locationId source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(const locationId source, Block & destination, const bool destIsPointer)
{
    const Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(const Block & source, const bool sourceIsPointer, const locationId destination)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(const Block & source, const bool sourceIsPointer, Block & desination, const bool destIsPointer)
{
    const Block * sourceBlock = sourceIsPointer ? getBlockFrom(source) : &source;
    if (sourceBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(desination) : &desination;
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(const Block * sourceBlock, Block * destBlock)
{
    if (sourceBlock->dataType() != destBlock->dataType()) return;

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() /= sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() /= sourceBlock->realData();
        break;

    default: break;
    }
}

////////////////////////////////////////////////////////////

void Machine::allocate(const Block::DataType dataType, const unsigned count)
{
    if (dataType == Block::DATA_TYPE_COUNT) return;
    managedHeap_.allocate(dataType, count, allocOutRegister_);
}

void Machine::allocate(const Block::DataType dataType, const locationId count)
{
    const Block * block = getBlockFrom(count);
    if ((block != NULL) && (block->dataType() == Block::DT_INTEGER)) allocate(dataType, block->integerData());
}

void Machine::allocate(Block::DataType dataType, const Block & count, bool countIsPointer)
{
    const Block * block = countIsPointer ? getBlockFrom(count) : &count;
    if ((block != NULL) && (block->dataType() == Block::DT_INTEGER)) allocate(dataType, block->integerData());
}

////////////////////////////////////////////////////////////

void Machine::getArrayElement(const locationId arrayPointer, const unsigned index)
{
    const Block * block = getBlockFrom(arrayPointer);
    if (block != NULL) getArrayElement(block, index);
}

void Machine::getArrayElement(const locationId arrayPointer, const locationId index)
{
    const Block * indexBlock = getBlockFrom(index);
    if ((indexBlock != NULL) && (indexBlock->dataType() == Block::DT_INTEGER))
        getArrayElement(arrayPointer, indexBlock->integerData());
}

void Machine::getArrayElement(locationId arrayPointer, const Block & index, const bool indexIsPointer)
{
    const Block * indexBlock = indexIsPointer ? getBlockFrom(index) : &index;
    if ((indexBlock != NULL) && (indexBlock->dataType() == Block::DT_INTEGER))
        getArrayElement(arrayPointer, indexBlock->integerData());
}

void Machine::getArrayElement(const Block & arrayPointer, const bool arrayPointerIsPointer, const unsigned index)
{
    const Block * block = arrayPointerIsPointer ? getBlockFrom(arrayPointer) : &arrayPointer;
    if (block != NULL) getArrayElement(block, index);
}

void Machine::getArrayElement(const Block & arrayPointer, const bool arrayPointerIsPointer, const locationId index)
{
    const Block * indexBlock = getBlockFrom(index);
    if ((indexBlock != NULL) && (indexBlock->dataType() == Block::DT_INTEGER))
        getArrayElement(arrayPointer, arrayPointerIsPointer, indexBlock->integerData());
}

void Machine::getArrayElement(const Block & arrayPointer, const bool arrayPointerIsPointer, const Block & index,
                              const bool indexIsPointer)
{
    const Block * indexBlock = indexIsPointer ? getBlockFrom(index) : &index;
    if ((indexBlock != NULL) && (indexBlock->dataType() == Block::DT_INTEGER))
        getArrayElement(arrayPointer, arrayPointerIsPointer, indexBlock->integerData());
}

void Machine::getArrayElement(const Block * pointerBlock, const unsigned index)
{
    if ((pointerBlock->dataType() != Block::DT_POINTER) || (index >= pointerBlock->pointerArrayLength())) return;
    const Block * element = pointerBlock->pointerArrayElementAt(index);
    if (element != NULL) allocOutRegister_ = *element;
}

////////////////////////////////////////////////////////////

void Machine::getArrayLength(locationId arrayPointer, locationId destination)
{
    const Block * pointerBlock = getBlockFrom(arrayPointer);
    if (pointerBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    getArrayLength(pointerBlock, destBlock);
}

void Machine::getArrayLength(locationId arrayPointer, Block & destination, bool destIsPointer)
{
    const Block * pointerBlock = getBlockFrom(arrayPointer);
    if (pointerBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    getArrayLength(pointerBlock, destBlock);
}

void Machine::getArrayLength(const Block & arrayPointer, bool arrayPointerIsPointer, locationId destination)
{
    const Block * pointerBlock = arrayPointerIsPointer ? getBlockFrom(arrayPointer) : &arrayPointer;
    if (pointerBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    getArrayLength(pointerBlock, destBlock);
}

void Machine::getArrayLength(const Block & arrayPointer, bool arrayPointerIsPointer, Block & destination,
                           bool destIsPointer)
{
    const Block * pointerBlock = arrayPointerIsPointer ? getBlockFrom(arrayPointer) : &arrayPointer;
    if (pointerBlock == NULL) return;
    Block * destBlock = destIsPointer ? getBlockFrom(destination) : &destination;
    if (destBlock == NULL) return;
    getArrayLength(pointerBlock, destBlock);
}

void Machine::getArrayLength(const Block * pointerBlock, Block * destBlock)
{
    if (pointerBlock->dataType() == Block::DT_POINTER) destBlock->setToInteger(pointerBlock->pointerArrayLength());
}

////////////////////////////////////////////////////////////

void Machine::compare(const locationId lhs, const locationId rhs)
{
    const Block * lhsBlock = getBlockFrom(lhs);
    if (lhsBlock == NULL) return;
    const Block * rhsBlock = getBlockFrom(rhs);
    if (rhsBlock == NULL) return;
    compare(lhsBlock, rhsBlock);
}

void Machine::compare(const locationId lhs, const Block & rhs, const bool rhsIsPointer)
{
    const Block * lhsBlock = getBlockFrom(lhs);
    if (lhsBlock == NULL) return;
    const Block * rhsBlock = rhsIsPointer ? getBlockFrom(rhs) : &rhs;
    if (rhsBlock == NULL) return;
    compare(lhsBlock, rhsBlock);
}

void Machine::compare(const Block & lhs, const bool lhsIsPointer, const locationId rhs)
{
    const Block * lhsBlock = lhsIsPointer ? getBlockFrom(lhs) : &lhs;
    if (lhsBlock == NULL) return;
    const Block * rhsBlock = getBlockFrom(rhs);
    if (rhsBlock == NULL) return;
    compare(lhsBlock, rhsBlock);
}

void Machine::compare(const Block & lhs, const bool lhsIsPointer, const Block & rhs, const bool rhsIsPointer)
{
    const Block * lhsBlock = lhsIsPointer ? getBlockFrom(lhs) : &lhs;
    if (lhsBlock == NULL) return;
    const Block * rhsBlock = rhsIsPointer ? getBlockFrom(rhs) : &rhs;
    if (rhsBlock == NULL) return;
    compare(lhsBlock, rhsBlock);
}

template<typename T1, typename T2>
void setInequalityComparisonFlags(const T1 lhs, const T2 rhs, ComparisonFlagRegister & flagRegister)
{
    flagRegister.setValue(ComparisonFlagRegister::F_LESS, lhs < rhs);
    flagRegister.setValue(ComparisonFlagRegister::F_GREATER, lhs > rhs);
    flagRegister.setValue(ComparisonFlagRegister::F_LESS_EQUAL, lhs <= rhs);
    flagRegister.setValue(ComparisonFlagRegister::F_GREATER_EQUAL, lhs >= rhs);
}

void Machine::compare(const Block * lhsBlock, const Block * rhsBlock)
{
    comparisonFlagRegister_.reset();

    bool equality = (*lhsBlock == *rhsBlock);
    comparisonFlagRegister_.setValue(ComparisonFlagRegister::F_EQUAL, equality);
    comparisonFlagRegister_.setValue(ComparisonFlagRegister::F_NOT_EQUAL, !equality);

    if (lhsBlock->dataType() != rhsBlock->dataType()) return;

    switch (lhsBlock->dataType())
    {
    case Block::DT_INTEGER:
        switch (rhsBlock->dataType())
        {
        case Block::DT_INTEGER:
            setInequalityComparisonFlags(lhsBlock->integerData(), rhsBlock->integerData(), comparisonFlagRegister_);
            break;
        case Block::DT_REAL:
            setInequalityComparisonFlags(lhsBlock->integerData(), rhsBlock->realData(), comparisonFlagRegister_);
            break;
        case Block::DT_CHAR:
            setInequalityComparisonFlags(lhsBlock->integerData(), rhsBlock->charData(), comparisonFlagRegister_);
            break;
        default: break;
        }
        break;

    case Block::DT_REAL:
        switch (rhsBlock->dataType())
        {
        case Block::DT_INTEGER:
            setInequalityComparisonFlags(lhsBlock->realData(), rhsBlock->integerData(), comparisonFlagRegister_);
            break;
        case Block::DT_REAL:
            setInequalityComparisonFlags(lhsBlock->realData(), rhsBlock->realData(), comparisonFlagRegister_);
            break;
        case Block::DT_CHAR:
            setInequalityComparisonFlags(lhsBlock->realData(), rhsBlock->charData(), comparisonFlagRegister_);
            break;
        default: break;
        }
        break;

    case Block::DT_CHAR:
        switch (rhsBlock->dataType())
        {
        case Block::DT_INTEGER:
            setInequalityComparisonFlags(lhsBlock->charData(), rhsBlock->integerData(), comparisonFlagRegister_);
            break;
        case Block::DT_REAL:
            setInequalityComparisonFlags(lhsBlock->charData(), rhsBlock->realData(), comparisonFlagRegister_);
            break;
        case Block::DT_CHAR:
            setInequalityComparisonFlags(lhsBlock->charData(), rhsBlock->charData(), comparisonFlagRegister_);
            break;
        default: break;
        }
        break;

    default: break;
    }
}

////////////////////////////////////////////////////////////

void Machine::copyFlag(const ComparisonFlagRegister::ComparisonFlagId flagId, locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL) block->setToBoolean(comparisonFlagRegister_.getValue(flagId));
}

void Machine::copyFlag(const ComparisonFlagRegister::ComparisonFlagId flagId, Block & destination,
                       const bool destIsPointer)
{
    Block * block = destIsPointer ? getBlockFrom(destination) : &destination;
    if (block != NULL) block->setToBoolean(comparisonFlagRegister_.getValue(flagId));
}

////////////////////////////////////////////////////////////

Stack & Machine::stack()
{
    return stack_;
}

Heap & Machine::unmanagedHeap()
{
    return unmanagedHeap_;
}

ManagedHeap & Machine::managedHeap()
{
    return managedHeap_;
}

ComparisonFlagRegister & Machine::comparisonFlagRegister()
{
    return comparisonFlagRegister_;
}

Block & Machine::primaryRegister()
{
    return primaryRegister_;
}

Block & Machine::allocOutRegister()
{
    return allocOutRegister_;
}

Block * Machine::getBlockFrom(const locationId location)
{
    switch (location)
    {
    case L_STACK:                   return &stack_.peek();
    case L_PRIMARY_REGISTER:        return &primaryRegister_;
    case L_ALLOC_OUT_REGISTER:      return &allocOutRegister_;
    case L_ADDR_STACK:              return getBlockFrom(stack_.peek());
    case L_ADDR_PRIMARY_REGISTER:   return getBlockFrom(primaryRegister_);
    case L_ADDR_ALLOC_OUT_REGISTER: return getBlockFrom(allocOutRegister_);
    default:                        return NULL;
    }
}

Block * Machine::getBlockFrom(const Block & pointer)
{
    if (pointer.dataType() != Block::DT_POINTER) return NULL;
    return &pointer.pointerHeap()->blockAt(pointer.pointerAddress());
}
