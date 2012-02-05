/*
 * Machine.cpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#include <cstdio>

#include "Machine.hpp"

const Machine::locationId Machine::STACK, Machine::PRIMARY_REGISTER, Machine::MANAGED_OUT_REGISTER, Machine::TRASH;

Machine::Machine(const unsigned stackSize, const unsigned unmanagedHeapSize, const unsigned managedHeapSize)
    : stack_(stackSize), unmanagedHeap_(unmanagedHeapSize), managedHeap_(managedHeapSize), programCounter_(0),
      operand1IsPointer_(false), operand2IsPointer_(false)
{
    returnAddressStack.reserve((stackSize == 0 ? Stack::defaultSize : stackSize) / 4);
}

void Machine::_clear(Block * locationBlock)
{
    if (locationBlock == NULL) return;
    locationBlock->clear();
}

void Machine::_set(const Block * value, Block * destBlock)
{
    if ((value == NULL) || (destBlock == NULL)) return;
    *destBlock = *value;
}

void Machine::_move(const Block * sourceBlock, Block * destBlock) // sacrificing DRYness for consistency...
{
    if ((sourceBlock == NULL) || (destBlock == NULL)) return;
    *destBlock = *sourceBlock;
}

void Machine::_read(Block * destBlock)
{
    if (destBlock == NULL) return;
    char c;
    if (scanf("%c", &c) != 0) destBlock->setToChar(c);
}

void Machine::_readString(Block * destBlock)
{
    if ((destBlock == NULL) || (destBlock->dataType() != Block::DT_POINTER)) return;

    Block * data = destBlock->pointerDataPointedTo();
    if ((data == NULL) || (data->dataType() != Block::DT_CHAR)) return;

    const unsigned length = destBlock->pointerArrayLength();
    if (length == 0)
    {
        char c;
        if (scanf("%c", &c) != 0) data->charData() = c;
    }
    else
    {
        char c;
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

void Machine::_write(const Block * sourceBlock)
{
    if (sourceBlock == NULL) return;
    std::cout << *sourceBlock << std::endl;
}

void Machine::_writeString(const Block * sourceBlock)
{
    if ((sourceBlock == NULL) || (sourceBlock->dataType() != Block::DT_POINTER)) return;

    Block * data = sourceBlock->pointerDataPointedTo();
    if ((data == NULL) || (data->dataType() != Block::DT_CHAR)) return;

    const unsigned length = sourceBlock->pointerArrayLength();
    if (length == 0) std::cout << data->charData() << std::endl;
    else
    {
        char c;
        for (unsigned i = 0; i < length; ++i)
        {
            c = sourceBlock->pointerArrayElementAt(i)->charData();
            if (c == '\0') break;
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

void Machine::_push(const Block * sourceBlock)
{
    if (sourceBlock == NULL) return;

    try
    {
        stack_.push(*sourceBlock);
    }
    catch(const std::exception & exception)
    {
        std::cout << exception.what() << std::endl;
    }
}

void Machine::_pop(Block * destBlock)
{
    if (destBlock == NULL) return;

    try
    {
        *destBlock = stack_.peek();
        stack_.pop();
    }
    catch(const std::exception & exception)
    {
        std::cout << exception.what() << std::endl;
    }
}

void Machine::_increment(Block * destBlock)
{
    if (destBlock == NULL) return;
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

void Machine::_decrement(Block * destBlock)
{
    if (destBlock == NULL) return;
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

void Machine::_add(const Block * sourceBlock, Block * destBlock)
{
    if ((sourceBlock == NULL) || (destBlock == NULL) || (sourceBlock->dataType() != destBlock->dataType())) return;

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

void Machine::_subtract(const Block * sourceBlock, Block * destBlock)
{
    if ((sourceBlock == NULL) || (destBlock == NULL) || (sourceBlock->dataType() != destBlock->dataType())) return;

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

void Machine::_multiply(const Block * sourceBlock, Block * destBlock)
{
    if ((sourceBlock == NULL) || (destBlock == NULL) || (sourceBlock->dataType() != destBlock->dataType())) return;

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

void Machine::_divide(const Block * sourceBlock, Block * destBlock)
{
    if ((sourceBlock == NULL) || (destBlock == NULL) || (sourceBlock->dataType() != destBlock->dataType())) return;

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

void Machine::allocateDirect(const Block::DataType dataType, const unsigned count)
{
    if (dataType == Block::DATA_TYPE_COUNT) return;

    try
    {
        managedHeap_.allocate(dataType, count, managedOutRegister_);
    }
    catch(const std::exception & exception)
    {
        std::cout << exception.what() << std::endl;
    }
}

void Machine::_allocate(const Block::DataType dataType, const Block * countBlock)
{std::cout << "hi" << std::endl;
    if ((countBlock == NULL) || (countBlock->dataType() != Block::DT_INTEGER)) return;
    allocateDirect(dataType, countBlock->integerData());
}

void Machine::_getArrayElement(const Block * pointerBlock, const unsigned index)
{
    if ((pointerBlock == NULL) || (pointerBlock->dataType() != Block::DT_POINTER) ||
            (index >= pointerBlock->pointerArrayLength())) return;
    const Block * element = pointerBlock->pointerArrayElementAt(index);
    if (element != NULL) managedOutRegister_ = *element;
}

void Machine::_getArrayElement(const Block * pointerBlock, const Block * indexBlock)
{
    if ((indexBlock == NULL) || (indexBlock->dataType() != Block::DT_INTEGER)) return;
    _getArrayElement(pointerBlock, indexBlock->integerData());
}

void Machine::_getArrayLength(const Block * pointerBlock, Block * destBlock)
{
    if ((pointerBlock == NULL) || (destBlock == NULL) || (pointerBlock->dataType() != Block::DT_POINTER)) return;
    destBlock->setToInteger(pointerBlock->pointerArrayLength());
}

template<typename T1, typename T2>
void setInequalityComparisonFlags(const T1 lhs, const T2 rhs, ComparisonFlagRegister & flagRegister)
{
    flagRegister.setValue(ComparisonFlagRegister::F_LESS, lhs < rhs);
    flagRegister.setValue(ComparisonFlagRegister::F_GREATER, lhs > rhs);
    flagRegister.setValue(ComparisonFlagRegister::F_LESS_EQUAL, lhs <= rhs);
    flagRegister.setValue(ComparisonFlagRegister::F_GREATER_EQUAL, lhs >= rhs);
}

void Machine::_compare(const Block * lhsBlock, const Block * rhsBlock)
{
    if ((lhsBlock == NULL) || (rhsBlock == NULL)) return;

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

void Machine::_copyFlag(const ComparisonFlagRegister::ComparisonFlagId flagId, Block * destBlock)
{
    if (destBlock == NULL) return;
    destBlock->setToBoolean(comparisonFlagRegister_.getValue(flagId));
}

void Machine::jump(const std::string & labelName)
{
    LabelTable::iterator iterator = labels_.find(labelName);
    if (iterator != labels_.end()) programCounter_ = iterator->second;
}

void Machine::conditionalJump(const ComparisonFlagRegister::ComparisonFlagId condition, const std::string & labelName)
{
    if (comparisonFlagRegister_.getValue(condition))
    {
        LabelTable::iterator iterator = labels_.find(labelName);
        if (iterator != labels_.end()) programCounter_ = iterator->second;
    }
}

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

const Machine::LabelTable & Machine::labels() const
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
    case L_STACK:
    {
        try
        {
            block = &stack_.peek();
        }
        catch(const std::exception & exception)
        {
            std::cout << exception.what() << std::endl;
            return NULL;
        }
    }
    break;

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

Block * Machine::getBlockFrom(const StackLocation location, const short operandNumber)
{
    Block * block;
    try
    {
        block = &stack_.at(location.value);
    }
    catch(const std::exception & exception)
    {
        std::cout << exception.what() << std::endl;
        return NULL;
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

        try
        {
            return &pointer.pointerHeap()->blockAt(pointer.pointerAddress());
        }
        catch(const std::exception & exception)
        {
            std::cout << exception.what() << std::endl;
            return NULL;
        }
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

        try
        {
            return &pointer.pointerHeap()->blockAt(pointer.pointerAddress());
        }
        catch(const std::exception & exception)
        {
            std::cout << exception.what() << std::endl;
            return NULL;
        }
    }

    return &pointer;
}
