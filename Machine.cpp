/*
 * Machine.cpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#include <cstdio>

#include "Machine.hpp"

Machine::Machine(const unsigned stackSize, const unsigned unmanagedHeapSize, const unsigned managedHeapSize)
    : stack_(stackSize), unmanagedHeap_(unmanagedHeapSize), managedHeap_(managedHeapSize), programCounter_(0),
      operand1IsPointer_(false), operand2IsPointer_(false) {}

void Machine::clear(const locationId location)
{
    Block * block = getBlockFrom(location, 1);
    if (block == NULL) return;
    clear(block);
}

void Machine::clear(Block & location)
{
    Block * block = getBlockFrom(location, 1);
    if (block != NULL) clear(block);
}

void Machine::clear(Block * locationBlock)
{
    locationBlock->clear();
}

////////////////////////////////////////////////////////////

void Machine::set(const Block & value, const locationId destination)
{
    const Block * valueBlock = getBlockFrom(value, 1);
    if (valueBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    set(valueBlock, destBlock);
}

void Machine::set(const Block & value, Block & destination)
{
    const Block * valueBlock = getBlockFrom(value, 1);
    if (valueBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    set(valueBlock, destBlock);
}

void Machine::set(const Block * value, Block * destBlock)
{
    *destBlock = *value;
}

////////////////////////////////////////////////////////////

void Machine::move(const locationId source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    move(sourceBlock, destBlock);
}

void Machine::move(const locationId source, Block & destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    move(sourceBlock, destBlock);
}

void Machine::move(const Block & source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    move(sourceBlock, destBlock);
}

void Machine::move(const Block & source, Block & destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
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
    Block * block = getBlockFrom(destination, 1);
    if (block != NULL) read(block);
}

void Machine::read(Block & destination)
{
    Block * block = getBlockFrom(destination, 1);
    if (block != NULL) read(block);
}

void Machine::read(Block * destBlock)
{
    char c;
    if (scanf("%c", &c) != 0) destBlock->setToChar(c);
}

////////////////////////////////////////////////////////////

void Machine::readString(const locationId destination)
{
    Block * block = getBlockFrom(destination, 1);
    if (block != NULL) readString(block);
}

void Machine::readString(Block & destination)
{
    Block * block = getBlockFrom(destination, 1);
    if (block != NULL) readString(block);
}

void Machine::readString(Block * destBlock)
{
    if (destBlock->dataType() != Block::DT_POINTER) return;
    Block * data = destBlock->pointerDataPointedTo();
    if ((data == NULL) || (data->dataType() != Block::DT_CHAR)) return;

    if (destBlock->pointerArrayLength() == 0)
    {
        char c;
        if (scanf("%c", &c) != 0) data->charData() = c;
    }
    else
    {
        char c;
        unsigned length = destBlock->pointerArrayLength();
        for (unsigned i = 0; i < length; ++i)
        {
            if (scanf("%c", &c) != 0)
            {
                if ((c == '\n') || (c == '\r') || (c== '\0'))
                {
                    destBlock->pointerArrayElementAt(i)->charData() = '\0';
                    break;
                }
                else destBlock->pointerArrayElementAt(i)->charData() = c;
            }
            else
            {
                destBlock->pointerArrayElementAt(i)->charData() = '\0';
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////

void Machine::write(const locationId source)
{
    const Block * block = getBlockFrom(source, 1);
    if (block != NULL) write(block);
}

void Machine::write(const Block & source)
{
    const Block * block = getBlockFrom(source, 1);
    if (block != NULL) write(block);
}

void Machine::write(const Block * sourceBlock)
{
    std::cout << *sourceBlock << std::endl;
}

////////////////////////////////////////////////////////////

void Machine::writeString(const locationId source)
{
    const Block * block = getBlockFrom(source, 1);
    if (block != NULL) writeString(block);
}

void Machine::writeString(const Block & source)
{
    const Block * block = getBlockFrom(source, 1);
    if (block != NULL) writeString(block);
}

void Machine::writeString(const Block * sourceBlock)
{
    if (sourceBlock->dataType() != Block::DT_POINTER) return;
    Block * data = sourceBlock->pointerDataPointedTo();
    if ((data == NULL) || (data->dataType() != Block::DT_CHAR)) return;

    if (sourceBlock->pointerArrayLength() == 0) std::cout << data->charData() << std::endl;
    else
    {
        char c;
        unsigned length = sourceBlock->pointerArrayLength();
        for (unsigned i = 0; i < length; ++i)
        {
            c = sourceBlock->pointerArrayElementAt(i)->charData();
            if (c == '\0') break;
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

////////////////////////////////////////////////////////////

void Machine::push(const locationId source)
{
    const Block * block = getBlockFrom(source, 1);
    if (block != NULL) push(block);
}

void Machine::push(const Block & source)
{
    const Block * block = getBlockFrom(source, 1);
    if (block != NULL) push(block);
}

void Machine::push(const Block * sourceBlock)
{
    stack_.push(*sourceBlock);
}

////////////////////////////////////////////////////////////

void Machine::pop(const locationId destination)
{
    Block * block = getBlockFrom(destination, 1);
    if (block != NULL) pop(block);
}

void Machine::pop(Block & destination)
{
    Block * block = getBlockFrom(destination, 1);
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
    Block * block = getBlockFrom(destination, 1);
    if (block != NULL) increment(block);
}

void Machine::increment(Block & destination)
{
    Block * block = getBlockFrom(destination, 1);
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
    Block * block = getBlockFrom(destination, 1);
    if (block != NULL) decrement(block);
}

void Machine::decrement(Block & destination)
{
    Block * block = getBlockFrom(destination, 1);
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
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const locationId source, Block & destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const Block & source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const Block & source, Block & destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
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
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const locationId source, Block & destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const Block & source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const Block & source, Block & destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
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
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const locationId source, Block & destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const Block & source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const Block & source, Block & destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
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
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(const locationId source, Block & destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(const Block & source, const locationId destination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(const Block & source, Block & desination)
{
    const Block * sourceBlock = getBlockFrom(source, 1);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(desination, 2);
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
    managedHeap_.allocate(dataType, count, managedOutRegister_);
}

void Machine::allocate(const Block::DataType dataType, const locationId count)
{
    const Block * block = getBlockFrom(count, 2);
    if ((block != NULL) && (block->dataType() == Block::DT_INTEGER)) allocate(dataType, block->integerData());
}

void Machine::allocate(Block::DataType dataType, const Block & count)
{
    const Block * block = getBlockFrom(count, 2);
    if ((block != NULL) && (block->dataType() == Block::DT_INTEGER)) allocate(dataType, block->integerData());
}

////////////////////////////////////////////////////////////

void Machine::getArrayElement(const locationId arrayPointer, const unsigned index)
{
    const Block * block = getBlockFrom(arrayPointer, 1);
    if (block != NULL) getArrayElement(block, index);
}

void Machine::getArrayElement(const locationId arrayPointer, const locationId index)
{
    const Block * indexBlock = getBlockFrom(index, 2);
    if ((indexBlock != NULL) && (indexBlock->dataType() == Block::DT_INTEGER))
        getArrayElement(arrayPointer, indexBlock->integerData());
}

void Machine::getArrayElement(locationId arrayPointer, const Block & index)
{
    const Block * indexBlock = getBlockFrom(index, 2);
    if ((indexBlock != NULL) && (indexBlock->dataType() == Block::DT_INTEGER))
        getArrayElement(arrayPointer, indexBlock->integerData());
}

void Machine::getArrayElement(const Block & arrayPointer, const unsigned index)
{
    const Block * block = getBlockFrom(arrayPointer, 1);
    if (block != NULL) getArrayElement(block, index);
}

void Machine::getArrayElement(const Block & arrayPointer, const locationId index)
{
    const Block * indexBlock = getBlockFrom(index, 2);
    if ((indexBlock != NULL) && (indexBlock->dataType() == Block::DT_INTEGER))
        getArrayElement(arrayPointer, indexBlock->integerData());
}

void Machine::getArrayElement(const Block & arrayPointer, const Block & index)
{
    const Block * indexBlock = getBlockFrom(index, 2);
    if ((indexBlock != NULL) && (indexBlock->dataType() == Block::DT_INTEGER))
        getArrayElement(arrayPointer, indexBlock->integerData());
}

void Machine::getArrayElement(const Block * pointerBlock, const unsigned index)
{
    if ((pointerBlock->dataType() != Block::DT_POINTER) || (index >= pointerBlock->pointerArrayLength())) return;
    const Block * element = pointerBlock->pointerArrayElementAt(index);
    if (element != NULL) managedOutRegister_ = *element;
}

////////////////////////////////////////////////////////////

void Machine::getArrayLength(locationId arrayPointer, locationId destination)
{
    const Block * pointerBlock = getBlockFrom(arrayPointer, 1);
    if (pointerBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    getArrayLength(pointerBlock, destBlock);
}

void Machine::getArrayLength(locationId arrayPointer, Block & destination)
{
    const Block * pointerBlock = getBlockFrom(arrayPointer, 1);
    if (pointerBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    getArrayLength(pointerBlock, destBlock);
}

void Machine::getArrayLength(const Block & arrayPointer, locationId destination)
{
    const Block * pointerBlock = getBlockFrom(arrayPointer, 1);
    if (pointerBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
    if (destBlock == NULL) return;
    getArrayLength(pointerBlock, destBlock);
}

void Machine::getArrayLength(const Block & arrayPointer, Block & destination)
{
    const Block * pointerBlock = getBlockFrom(arrayPointer, 1);
    if (pointerBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination, 2);
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
    const Block * lhsBlock = getBlockFrom(lhs, 1);
    if (lhsBlock == NULL) return;
    const Block * rhsBlock = getBlockFrom(rhs, 2);
    if (rhsBlock == NULL) return;
    compare(lhsBlock, rhsBlock);
}

void Machine::compare(const locationId lhs, const Block & rhs)
{
    const Block * lhsBlock = getBlockFrom(lhs, 1);
    if (lhsBlock == NULL) return;
    const Block * rhsBlock = getBlockFrom(rhs, 2);
    if (rhsBlock == NULL) return;
    compare(lhsBlock, rhsBlock);
}

void Machine::compare(const Block & lhs, const locationId rhs)
{
    const Block * lhsBlock = getBlockFrom(lhs, 1);
    if (lhsBlock == NULL) return;
    const Block * rhsBlock = getBlockFrom(rhs, 2);
    if (rhsBlock == NULL) return;
    compare(lhsBlock, rhsBlock);
}

void Machine::compare(const Block & lhs, const Block & rhs)
{
    const Block * lhsBlock = getBlockFrom(lhs, 1);
    if (lhsBlock == NULL) return;
    const Block * rhsBlock = getBlockFrom(rhs, 2);
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
    Block * block = getBlockFrom(destination, 2);
    if (block != NULL) block->setToBoolean(comparisonFlagRegister_.getValue(flagId));
}

void Machine::copyFlag(const ComparisonFlagRegister::ComparisonFlagId flagId, Block & destination)
{
    Block * block = getBlockFrom(destination, 2);
    if (block != NULL) block->setToBoolean(comparisonFlagRegister_.getValue(flagId));
}

////////////////////////////////////////////////////////////

void Machine::jump(const std::string & labelName)
{
    labelTable::iterator iterator = labels_.find(labelName);
    if (iterator != labels_.end()) programCounter_ = iterator->second;
}

void Machine::conditionalJump(const ComparisonFlagRegister::ComparisonFlagId condition, const std::string & labelName)
{
    if (comparisonFlagRegister_.getValue(condition))
    {
        labelTable::iterator iterator = labels_.find(labelName);
        if (iterator != labels_.end()) programCounter_ = iterator->second;
    }
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

Block & Machine::managedOutRegister()
{
    return managedOutRegister_;
}

unsigned & Machine::programCounter()
{
    return programCounter_;
}

const Machine::labelTable & Machine::labels() const
{
    return labels_;
}

void Machine::addLabel(const std::string & labelName, unsigned lineNumber)
{
    if (labelName.empty()) return;
    labels_[labelName] = lineNumber;
}

bool & Machine::operand1IsPointer()
{
    return operand1IsPointer_;
}

bool Machine::operand1IsPointer() const
{
    return operand1IsPointer_;
}

bool & Machine::operand2IsPointer()
{
    return operand2IsPointer_;
}

bool Machine::operand2IsPointer() const
{
    return operand2IsPointer_;
}

Block * Machine::getBlockFrom(const locationId location, const short operandNumber)
{
    Block * block;
    switch (location)
    {
    case L_STACK:                block = &stack_.peek(); break;
    case L_PRIMARY_REGISTER:     block = &primaryRegister_; break;
    case L_MANAGED_OUT_REGISTER: block = &managedOutRegister_; break;
    default:                     block = NULL;
    }

    if (block == NULL) return NULL;

    bool returnReferencedBlock;
    switch (operandNumber)
    {
    case 1:  returnReferencedBlock = operand1IsPointer_; break;
    case 2:  returnReferencedBlock = operand2IsPointer_; break;
    case 3:  returnReferencedBlock = true; break;
    default: return block;
    }

    if (returnReferencedBlock) return getBlockFrom(*block , 3);
    return block;
}

Block * Machine::getBlockFrom(Block & pointer, const short operandNumber)
{
    bool returnReferencedBlock;
    switch (operandNumber)
    {
    case 1: returnReferencedBlock = operand1IsPointer_; break;
    case 2: returnReferencedBlock = operand2IsPointer_; break;
    case 3: returnReferencedBlock = true;
    default: return &pointer;
    }

    if (returnReferencedBlock)
    {
        if (pointer.dataType() != Block::DT_POINTER) return NULL;
        return &pointer.pointerHeap()->blockAt(pointer.pointerAddress());
    }

    return &pointer;
}

const Block * Machine::getBlockFrom(const Block & pointer, const short operandNumber)
{
    bool returnReferencedBlock;
    switch (operandNumber)
    {
    case 1: returnReferencedBlock = operand1IsPointer_; break;
    case 2: returnReferencedBlock = operand2IsPointer_; break;
    case 3: returnReferencedBlock = true;
    default: return &pointer;
    }

    if (returnReferencedBlock)
    {
        if (pointer.dataType() != Block::DT_POINTER) return NULL;
        return &pointer.pointerHeap()->blockAt(pointer.pointerAddress());
    }

    return &pointer;
}
