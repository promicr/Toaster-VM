/*
 * Machine.cpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#include <cstdio>
#include <stdexcept>
#include <cmath>
#include <dlfcn.h>

#include "Machine.hpp"
#include "ExtensionFunction.hpp"

const Machine::locationId Machine::STACK, Machine::PRIMARY_REGISTER, Machine::MANAGED_OUT_REGISTER,
Machine::EXTENSION_OUT_REGISTER, Machine::NIL;

std::vector<void*> Machine::extensionHandles;
unsigned Machine::machineCount = 0;
const unsigned Machine::extensionMachineStackSize = 1000, Machine::extensionMachineHeapSize = 1000;

Machine::Machine(const unsigned stackSize, const unsigned unmanagedHeapSize, const unsigned managedHeapSize)
    : stack_(stackSize), unmanagedHeap_(unmanagedHeapSize), managedHeap_(managedHeapSize), programCounter_(0),
      operand1IsPointer_(false), operand2IsPointer_(false), extensionMachine(NULL)
{
    returnAddressStack.reserve((stackSize == 0 ? Stack::defaultSize : stackSize) / 4);
    ++machineCount;
}

Machine::~Machine()
{
    if (extensionMachine != NULL) delete extensionMachine;
    if (--machineCount == 0) for (unsigned i = 0; i < extensionHandles.size(); ++i) dlclose(extensionHandles[i]);
}

Machine::ArrayPopulator::ArrayPopulator()
    : arrayPointer(Block::DT_POINTER), currentIndex(-1) {}

void Machine::ArrayPopulator::start(const Block & pointerToArray)
{
    if (pointerToArray.dataType() != Block::DT_POINTER)
        throw(std::runtime_error("Machine::ArrayPopulator::start: Argument data type is invalid (pointer expected)"));
    if (pointerToArray.pointerIsNull())
        throw(std::runtime_error("Machine::ArrayPopulator::start: Array pointer given is null"));
    arrayPointer = pointerToArray;
}

void Machine::ArrayPopulator::stop()
{
    arrayPointer.nullifyPointerData();
    currentIndex = -1;
}

void Machine::ArrayPopulator::add(const Block & value)
{
    if (arrayPointer.pointerIsNull())
        throw(std::runtime_error("Machine::ArrayPopulator::add: No array has been specified to populate"));
    Block * element = arrayPointer.pointerArrayElementAt(++currentIndex);
    if (element == NULL) throw(std::runtime_error("Machine::ArrayPopulator::add: Array index out of range"));
    *element = value;
}

void Machine::_clear(Block * locationBlock)
{
    if (locationBlock == NULL) throw(std::runtime_error("Machine::_clear: Invalid location given"));
    locationBlock->clear();
}

void Machine::_set(const Block * value, Block * destBlock)
{
    if (value == NULL) destBlock->clear();
    else if (destBlock == NULL) throw(std::runtime_error("Machine::_move: Invalid destination given"));
    *destBlock = *value;
}

void Machine::_move(const Block * sourceBlock, Block * destBlock) // sacrificing DRYness for consistency...
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_move: Invalid source given"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_move: Invalid destination given"));
    *destBlock = *sourceBlock;
}

void Machine::_swap(Block * a, Block * b)
{
    if (a == NULL) throw(std::runtime_error("Machine::_exchange: Invalid first argument given"));
    if (b == NULL) throw(std::runtime_error("Machine::_exchange: Invalid second argument given"));

    Block temp(*a);
    *a = *b;
    *b = temp;
}

void Machine::_read(Block * destBlock)
{
    if (destBlock == NULL) throw(std::runtime_error("Machine::_read: Invalid destination given"));
    char c;
    if (scanf("%c", &c) != 0) destBlock->setToChar(c);
}

void Machine::_readString(Block * destBlock)
{
    if (destBlock == NULL) throw(std::runtime_error("Machine::_readString: Invalid destination given"));
    if (destBlock->dataType() != Block::DT_POINTER)
        throw(std::runtime_error("Machine::_readString: Data type of destination is invalid (pointer expected)"));

    Block * data = destBlock->pointerDataPointedTo();
    if (data == NULL) throw(std::runtime_error("Machine::_readString: Destination pointer is null"));
    if (data->dataType() != Block::DT_CHAR)
        throw(std::runtime_error("Machine::_readString: Destination pointer does not point to an array of characters"));

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
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_write: Invalid source given"));
    std::cout << *sourceBlock << std::endl;
}

void Machine::_writeString(const Block * sourceBlock)
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_writeString: Invalid source given"));
    if (sourceBlock->dataType() != Block::DT_POINTER)
        throw(std::runtime_error("Machine::_writeString: Data type of source is invalid (pointer expected)"));

    Block * data = sourceBlock->pointerDataPointedTo();
    if (data == NULL) throw(std::runtime_error("Machine::_writeString: Destination pointer is null"));
    if (data->dataType() != Block::DT_CHAR)
        throw(std::runtime_error(
                "Machine::_writeString: Destination pointer does not point to an array of characters"));

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
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_push: Invalid source given"));
    stack_.push(*sourceBlock);
}

void Machine::_pop(Block * destBlock)
{
    if (destBlock == NULL)
    {
        stack_.pop();
        return;
    }

    *destBlock = stack_.peek();
    stack_.pop();
}

void Machine::_increment(Block * destBlock)
{
    if (destBlock == NULL) throw(std::runtime_error("Machine::_increment: Invalid destination given"));
    switch (destBlock->dataType())
    {
    case Block::DT_INTEGER: ++destBlock->integerData(); break;
    case Block::DT_REAL:    ++destBlock->realData(); break;
    case Block::DT_CHAR:    ++destBlock->charData(); break;
    case Block::DT_POINTER:
        if (!destBlock->pointerIsNull() && (destBlock->pointerAddress() < (int)destBlock->pointerHeap()->size()))
            ++destBlock->pointerAddress();
        break;

    default: throw(std::runtime_error("Machine::_increment: Destination data type is invalid"));
    }
}

void Machine::_decrement(Block * destBlock)
{
    if (destBlock == NULL) throw(std::runtime_error("Machine::_decrement: Invalid destination given"));
    switch (destBlock->dataType())
    {
    case Block::DT_INTEGER: --destBlock->integerData(); break;
    case Block::DT_REAL:    --destBlock->realData(); break;
    case Block::DT_CHAR:    --destBlock->charData(); break;
    case Block::DT_POINTER:
        if (!destBlock->pointerIsNull() && (destBlock->pointerAddress() > 0))
            --destBlock->pointerAddress();
        break;

    default: throw(std::runtime_error("Machine::_decrement: Destination data type is invalid"));
    }
}

void Machine::_negate(Block * destBlock)
{
    if (destBlock == NULL) throw(std::runtime_error("Machine::_negate: Invalid destination given"));
    switch (destBlock->dataType())
    {
    case Block::DT_INTEGER: destBlock->integerData() = -destBlock->integerData(); break;
    case Block::DT_REAL:    destBlock->realData() = -destBlock->realData(); break;
    default: throw(std::runtime_error("Machine::_negate: Destination data type is invalid (expected integer or real)"));
    }
}

void Machine::_absolute(Block * destBlock)
{
    if (destBlock == NULL) throw(std::runtime_error("Machine::_absolute: Invalid destination given"));
    switch (destBlock->dataType())
    {
    case Block::DT_INTEGER: destBlock->integerData() = std::abs(destBlock->integerData()); break;
    case Block::DT_REAL:    destBlock->realData() = fabs(destBlock->realData()); break;
    default:
        throw(std::runtime_error("Machine::_absolute: Destination data type is invalid (expected integer or real)"));
    }
}

void Machine::_add(const Block * sourceBlock, Block * destBlock)
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_add: Invalid source given"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_add: Invalid destination given"));
    if (sourceBlock->dataType() != destBlock->dataType())
        throw(std::runtime_error("Machine::_add: Data types of operands do not match"));

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() += sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() += sourceBlock->realData();
        break;

    default: throw(std::runtime_error("Machine::_add: Data types of operands are invalid (expected integer or real)"));
    }
}

void Machine::_subtract(const Block * sourceBlock, Block * destBlock)
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_subtract: Invalid source given"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_subtract: Invalid destination given"));
    if (sourceBlock->dataType() != destBlock->dataType())
        throw(std::runtime_error("Machine::_subtract: Data types of operands do not match"));

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() -= sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() -= sourceBlock->realData();
        break;

    default:
        throw(std::runtime_error("Machine::_subtract: Data types of operands are invalid (expected integer or real)"));
    }
}

void Machine::_multiply(const Block * sourceBlock, Block * destBlock)
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_multiply: Invalid source given"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_multiply: Invalid destination given"));
    if (sourceBlock->dataType() != destBlock->dataType())
        throw(std::runtime_error("Machine::_multiply: Data types of operands do not match"));

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() *= sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() *= sourceBlock->realData();
        break;

    default:
        throw(std::runtime_error("Machine::_multiply: Data types of operands are invalid (expected integer or real)"));
    }
}

void Machine::_divide(const Block * sourceBlock, Block * destBlock)
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_divide: Invalid source given"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_divide: Invalid destination given"));
    if (sourceBlock->dataType() != destBlock->dataType())
        throw(std::runtime_error("Machine::_divide: Data types of operands do not match"));

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() /= sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() /= sourceBlock->realData();
        break;

    default:
        throw(std::runtime_error("Machine::_divide: Data types of operands are invalid (expected integer or real)"));
    }
}

void Machine::_modlulo(const Block * sourceBlock, Block * destBlock)
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_modlulo: Invalid source given"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_modlulo: Invalid destination given"));
    if (sourceBlock->dataType() != destBlock->dataType())
        throw(std::runtime_error("Machine::_modlulo: Data types of operands do not match"));

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() = destBlock->integerData() % sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() = fmod(destBlock->realData(), sourceBlock->realData());
        break;

    default:
        throw(std::runtime_error("Machine::_modlulo: Data types of operands are invalid (expected integer or real)"));
    }
}

void Machine::stackAdd()
{
    if ((stack_.empty()) || (stack_.count() < 2))
        throw(std::runtime_error("Machine::stackAdd: Not enough items on stack (minimum 2)"));
    _add(&stack_.fromTop(0), &stack_.fromTop(1));
    stack_.pop();
}

void Machine::stackSubtract()
{
    if ((stack_.empty()) || (stack_.count() < 2))
        throw(std::runtime_error("Machine::stackSubtract: Not enough items on stack (minimum 2)"));
    _subtract(&stack_.fromTop(0), &stack_.fromTop(1));
    stack_.pop();
}

void Machine::stackMultiply()
{
    if ((stack_.empty()) || (stack_.count() < 2))
        throw(std::runtime_error("Machine::stackMultiply: Not enough items on stack (minimum 2)"));
    _multiply(&stack_.fromTop(0), &stack_.fromTop(1));
    stack_.pop();
}

void Machine::stackDivide()
{
    if ((stack_.empty()) || (stack_.count() < 2))
        throw(std::runtime_error("Machine::stackDivide: Not enough items on stack (minimum 2)"));
    _divide(&stack_.fromTop(0), &stack_.fromTop(1));
    stack_.pop();
}

void Machine::stackModulo()
{
    if ((stack_.empty()) || (stack_.count() < 2))
        throw(std::runtime_error("Machine::stackModulo: Not enough items on stack (minimum 2)"));
    _modlulo(&stack_.fromTop(0), &stack_.fromTop(1));
    stack_.pop();
}

void Machine::allocateDirect(const Block::DataType dataType, const unsigned count, Block & pointerDestination)
{
    if (dataType == Block::DATA_TYPE_COUNT)
        throw(std::runtime_error("Machine::allocateDirect: Invalid data type given"));
    managedHeap_.allocate(dataType, count, pointerDestination);
}

void Machine::_allocate(const Block::DataType dataType, const Block * countBlock)
{
    if (countBlock == NULL) throw(std::runtime_error("Machine::_allocate: Invalid array size given"));
    if (countBlock->dataType() != Block::DT_INTEGER)
        throw(std::runtime_error("Machine::_allocate: Data type of array size is invalid (expected integer)"));
    allocateDirect(dataType, countBlock->integerData(), managedOutRegister_);
}

void Machine::_startPopulatingArray(const Block * pointerBlock)
{
    if (pointerBlock == NULL) throw(std::runtime_error("Machine::_startPopulatingArray: Invalid array pointer given"));
    arrayBeingPopulated.start(*pointerBlock);
}

void Machine::stopPopulatingArray()
{
    arrayBeingPopulated.stop();
}

void Machine::_addToArray(const Block * valueBlock)
{
    if (valueBlock == NULL) throw(std::runtime_error("Machine::_addToArray: Invalid value given"));
    arrayBeingPopulated.add(*valueBlock);
}

void Machine::_getArrayElement(const Block * pointerBlock, const unsigned index)
{
    if (pointerBlock == NULL) throw(std::runtime_error("Machine::_getArrayElement: Invalid array pointer given"));
    if (pointerBlock->dataType() != Block::DT_POINTER)
        throw(std::runtime_error("Machine::_getArrayElement: First argument data type is invalid (expected pointer)"));
    if (index >= pointerBlock->pointerArrayLength())
        throw(std::runtime_error("Machine::_getArrayElement: Index given is out of array range"));
    const Block * element = pointerBlock->pointerArrayElementAt(index);
    if (element != NULL) managedOutRegister_ = *element;
}

void Machine::_getArrayElement(const Block * pointerBlock, const Block * indexBlock)
{
    if (indexBlock == NULL)
        throw(std::runtime_error("Machine::_getArrayElement: Element index is invalid"));
    if (indexBlock->dataType() != Block::DT_INTEGER)
        throw(std::runtime_error("Machine::_getArrayElement: Element index data type is invalid (expected integer)"));;
    _getArrayElement(pointerBlock, indexBlock->integerData());
}

void Machine::_getArrayLength(const Block * pointerBlock, Block * destBlock)
{
    if (pointerBlock == NULL) throw(std::runtime_error("Machine::_getArrayLength: Array pointer is invalid"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_getArrayLength: Destination is invalid"));
    if (pointerBlock->dataType() != Block::DT_POINTER)
        throw(std::runtime_error("Machine::_getArrayLength: First argument data type is invalid (expected pointer)"));
    destBlock->setToInteger(pointerBlock->pointerArrayLength());
}

void Machine::_copyArray(const Block * sourcePointerBlock, const Block * destPointerBlock)
{
    if (sourcePointerBlock == NULL)
        throw(std::runtime_error("Machine::_copyArray: Source array pointer is invalid"));
    if (destPointerBlock == NULL)
        throw(std::runtime_error("Machine::_copyArray: Destination array pointer is invalid"));
    if (sourcePointerBlock->dataType() != Block::DT_POINTER)
        throw(std::runtime_error("Machine::_copyArray: Source data type is invalid (expected pointer)"));
    if (destPointerBlock->dataType() != Block::DT_POINTER)
        throw(std::runtime_error("Machine::_copyArray: Destination data type is invalid (expected pointer)"));
    if (sourcePointerBlock->pointerIsNull())
        throw(std::runtime_error("Machine::_copyArray: Source array pointer is null"));
    if (destPointerBlock->pointerIsNull())
        throw(std::runtime_error("Machine::_copyArray: Destination array pointer is null"));

    const unsigned length = sourcePointerBlock->pointerArrayLength();
    if (length > destPointerBlock->pointerArrayLength())
        throw(std::runtime_error("Machine::_copyArray: Source array being copied is larger than the destination"));

    Block * sourceBlock, * destBlock;
    for (unsigned i = 0; i < length; ++i)
    {
        sourceBlock = sourcePointerBlock->pointerArrayElementAt(i);
        if (sourceBlock == NULL) throw(std::runtime_error("Machine::_copyArray: Error with source array"));
        destBlock = destPointerBlock->pointerArrayElementAt(i);
        if (destBlock == NULL) throw(std::runtime_error("Machine::_copyArray: Error with destination array"));;

        *destBlock = *sourceBlock;
    }
}

void Machine::_convert(const Block * sourceBlock, Block * destBlock, const Block::DataType dataType)
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_convert: Invalid source given"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_convert: Invalid destination given"));
    if (dataType == Block::DATA_TYPE_COUNT) throw(std::runtime_error("Machine::_convert: Invalid data type given"));
    if (destBlock->dataType() == Block::DT_POINTER)
        throw(std::runtime_error("Machine::_convert: Pointers cannot be converted into other data types"));

    switch (dataType)
    {
    case Block::DT_INTEGER:
        switch (sourceBlock->dataType())
        {
        case Block::DT_INTEGER: destBlock->setToInteger(sourceBlock->integerData()); break;
        case Block::DT_REAL:    destBlock->setToInteger(sourceBlock->realData()); break;
        case Block::DT_CHAR:    destBlock->setToInteger(sourceBlock->charData()); break;
        case Block::DT_BOOLEAN: destBlock->setToInteger(sourceBlock->booleanData()); break;
        default: break;
        }
        break;
    case Block::DT_REAL:
        switch (sourceBlock->dataType())
        {
        case Block::DT_INTEGER: destBlock->setToReal(sourceBlock->integerData()); break;
        case Block::DT_REAL:    destBlock->setToReal(sourceBlock->realData()); break;
        case Block::DT_CHAR:    destBlock->setToReal(sourceBlock->charData()); break;
        case Block::DT_BOOLEAN: destBlock->setToReal(sourceBlock->booleanData()); break;
        default: break;
        }
        break;
    case Block::DT_CHAR:
        switch (sourceBlock->dataType())
        {
        case Block::DT_INTEGER: destBlock->setToChar(sourceBlock->integerData()); break;
        case Block::DT_REAL:    destBlock->setToChar(sourceBlock->realData()); break;
        case Block::DT_CHAR:    destBlock->setToChar(sourceBlock->charData()); break;
        case Block::DT_BOOLEAN: destBlock->setToChar(sourceBlock->booleanData()); break;
        default: break;
        }
        break;
    case Block::DT_BOOLEAN:
        switch (sourceBlock->dataType())
        {
        case Block::DT_INTEGER: destBlock->setToBoolean(sourceBlock->integerData()); break;
        case Block::DT_REAL:    destBlock->setToBoolean(sourceBlock->realData()); break;
        case Block::DT_CHAR:    destBlock->setToBoolean(sourceBlock->charData()); break;
        case Block::DT_BOOLEAN: destBlock->setToBoolean(sourceBlock->booleanData()); break;
        default: break;
        }
        break;
    default: throw(std::runtime_error("Machine::_convert: Cannot convert other data types into pointers"));
    }
}

void Machine::_convertToDataTypeOf(Block * destBlock, const Block * sourceBlock)
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_convertToDataTypeOf: Invalid data type source given"));
    _convert(destBlock, destBlock, sourceBlock->dataType());
}

void Machine::_dereference(const Block * pointerBlock, Block * destBlock)
{
    if (pointerBlock == NULL) throw(std::runtime_error("Machine::_dereference: Pointer is invalid"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_dereference: Destination is invalid"));
    if (pointerBlock->pointerIsNull()) throw(std::runtime_error("Machine::_dereference: Pointer is null"));
    const Block * block = pointerBlock->pointerDataPointedTo();
    if (block == NULL) throw(std::runtime_error("Machine::_dereference: Pointer is null"));
    *destBlock = *block;
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
    if (lhsBlock == NULL) throw(std::runtime_error("Machine::_compare: First argument is invalid"));
    if (rhsBlock == NULL) throw(std::runtime_error("Machine::_compare: Second argument is invalid"));
    comparisonFlagRegister_.reset();

    const bool equality = (*lhsBlock == *rhsBlock);
    comparisonFlagRegister_.setValue(ComparisonFlagRegister::F_EQUAL, equality);
    comparisonFlagRegister_.setValue(ComparisonFlagRegister::F_NOT_EQUAL, !equality);

    if (lhsBlock->dataType() != rhsBlock->dataType()) return; // Don't throw exception here

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

void Machine::_compareDataType(const Block * lhsBlock, const Block * rhsBlock)
{
    if (lhsBlock == NULL) throw(std::runtime_error("Machine::_compare: First argument is invalid"));
    if (rhsBlock == NULL) throw(std::runtime_error("Machine::_compare: Second argument is invalid"));
    comparisonFlagRegister_.reset();

    const bool equality = (lhsBlock->dataType() == rhsBlock->dataType());
    comparisonFlagRegister_.setValue(ComparisonFlagRegister::F_EQUAL, equality);
    comparisonFlagRegister_.setValue(ComparisonFlagRegister::F_NOT_EQUAL, !equality);
}

void Machine::_isDataType(const Block * block, const Block::DataType dataType)
{
    if (block == NULL) throw(std::runtime_error("Machine::_compare: First argument is invalid"));
    comparisonFlagRegister_.reset();

    const bool equality = (block->dataType() == dataType);
    comparisonFlagRegister_.setValue(ComparisonFlagRegister::F_EQUAL, equality);
    comparisonFlagRegister_.setValue(ComparisonFlagRegister::F_NOT_EQUAL, !equality);
}

void Machine::_copyFlag(const ComparisonFlagRegister::ComparisonFlagId flagId, Block * destBlock)
{
    if (destBlock == NULL) throw(std::runtime_error("Machine::_copyFlag: Destination argument is invalid"));
    destBlock->setToBoolean(comparisonFlagRegister_.getValue(flagId));
}

void Machine::_logicalNot(Block * destBlock)
{
    if (destBlock == NULL) throw(std::runtime_error("Machine::_logicalAnd: Destination is invalid"));
    if (destBlock->dataType() != Block::DT_BOOLEAN)
        throw(std::runtime_error("Machine::_logicalAnd: Destination data type is invalid (boolean expected)"));
    destBlock->setToBoolean(!destBlock->booleanData());
}

void validateBooleans(const Block * sourceBlock, const Block * destBlock)
{
    if (sourceBlock == NULL) throw(std::runtime_error("Machine::_logicalAnd: Source is invalid"));
    if (destBlock == NULL) throw(std::runtime_error("Machine::_logicalAnd: Destination is invalid"));
    if (sourceBlock->dataType() != Block::DT_BOOLEAN)
        throw(std::runtime_error("Machine::_logicalAnd: Source data type is invalid (boolean expected)"));
    if (destBlock->dataType() != Block::DT_BOOLEAN)
        throw(std::runtime_error("Machine::_logicalAnd: Destination data type is invalid (boolean expected)"));
}

void Machine::_logicalAnd(const Block * sourceBlock, Block * destBlock)
{
    validateBooleans(sourceBlock, destBlock);
    destBlock->setToBoolean(sourceBlock->booleanData() && destBlock->booleanData());
}

void Machine::_logicalOr(const Block * sourceBlock, Block * destBlock)
{
    validateBooleans(sourceBlock, destBlock);
    destBlock->setToBoolean(sourceBlock->booleanData() || destBlock->booleanData());
}

void Machine::_logicalXor(const Block * sourceBlock, Block * destBlock)
{
    validateBooleans(sourceBlock, destBlock);
    destBlock->setToBoolean(sourceBlock->booleanData() != destBlock->booleanData());
}

void Machine::jump(const std::string & labelName)
{
    LabelTable::iterator iterator = labels_.find(labelName);
    if (iterator == labels_.end()) throw(std::runtime_error("Machine::jump: Unknown label '" + labelName + "'"));
    programCounter_ = iterator->second;
}

void Machine::conditionalJump(const ComparisonFlagRegister::ComparisonFlagId condition, const std::string & labelName)
{
    if (comparisonFlagRegister_.getValue(condition)) jump(labelName);
}

void Machine::call(const std::string & labelName)
{
    const unsigned returnAddress = programCounter_ + 1;
    jump(labelName);
    returnAddressStack.push_back(returnAddress);
    stack_.pushFrame();
}

void Machine::_returnFromCall(const Block * returnBlock)
{
    stack_.popFrame(returnBlock);
    programCounter_ = returnAddressStack.back();
    returnAddressStack.pop_back();
}

void Machine::loadExtension(const std::string & fileName)
{
    void * handle = dlopen(fileName.c_str(), RTLD_LAZY);
    char * error = dlerror();
    if ((handle == NULL) || (error != NULL))
        throw(std::runtime_error("Machine::loadExtension: " + std::string(error)));

    void * f = dlsym(handle, "tvmLoadExtension");
    error = dlerror();
    if ((f == NULL) || (error != NULL)) throw(std::runtime_error("Machine::loadExtension: " + std::string(error)));

    typedef void(*AddNewFunctionFunction)(const char *, ExtensionFunction::Pointer, unsigned);
    typedef void(*LoaderFunction)(AddNewFunctionFunction);
    LoaderFunction loadFunction = *reinterpret_cast<LoaderFunction*>(&f);
    loadFunction(ExtensionFunction::addNew);

    extensionHandles.push_back(handle);
}

void Machine::extensionCall(const std::string & functionName)
{
    ExtensionFunction * function = ExtensionFunction::find(functionName);
    if (function == NULL)
        throw(std::runtime_error("Machine::extensionCall: Unknown extension function '" + functionName + "'"));

    if (extensionMachine == NULL)
        extensionMachine = new Machine(extensionMachineStackSize, extensionMachineHeapSize, extensionMachineHeapSize);
    const Block block = function->call(stack_, extensionMachine);

    _returnFromCall(&block);
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

Block * Machine::getBlockFrom(const StackLocation location, const short operandNumber)
{
    Block * block = &stack_.at(location.value);

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
    case 1:  returnReferencedBlock = operand1IsPointer_; break;
    case 2:  returnReferencedBlock = operand2IsPointer_; break;
    case 3:  returnReferencedBlock = true; break;
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
    case 1:  returnReferencedBlock = operand1IsPointer_; break;
    case 2:  returnReferencedBlock = operand2IsPointer_; break;
    case 3:  returnReferencedBlock = true; break;
    default: return &pointer;
    }

    if (returnReferencedBlock)
    {
        if (pointer.dataType() != Block::DT_POINTER) return NULL;
        return &pointer.pointerHeap()->blockAt(pointer.pointerAddress());
    }

    return &pointer;
}
