/*
 * Machine.hpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <map>
#include <vector>

#include "Stack.hpp"
#include "ManagedHeap.hpp"
#include "ComparisonFlagRegister.hpp"

class Machine
{
public:
    typedef std::map<std::string, unsigned> LabelTable; // Maybe change string to int, i.e. hashed label name
    typedef std::vector<unsigned> ReturnAddressStack;

    struct StackLocation
    {
        unsigned value;
        explicit StackLocation(const unsigned value) : value(value) {}
        operator unsigned() const { return value; }
        StackLocation & operator =(const unsigned rhs) { value = rhs; return *this; }
    };

    struct HeapLocation
    {
        unsigned value;
        explicit HeapLocation(const unsigned value) : value(value) {}
        operator unsigned() const { return value; }
        HeapLocation & operator =(const unsigned rhs) { value = rhs; return *this; }
    };

    enum locationId
    {
        L_STACK,
        L_PRIMARY_REGISTER,
        L_MANAGED_OUT_REGISTER,
        L_NIL
    }
    static const STACK = L_STACK, // For use in Machine member functions that take T& arguments
    PRIMARY_REGISTER = L_PRIMARY_REGISTER,
    MANAGED_OUT_REGISTER = L_MANAGED_OUT_REGISTER,
    NIL = L_NIL;

    Machine(unsigned stackSize = 0, unsigned unmanagedHeapSize = 0, unsigned managedHeapSize = 0);
    ~Machine();

    // Member functions relating to the bytecode
    template <typename T>
    void clear(T & location);
    template <typename T>
    void clear(const T & location);

    template <typename T1, typename T2>
    void set(T1 & destination, const T2 & value);
    template <typename T1, typename T2>
    void set(const T1 & destination, const T2 & value);

    template <typename T1, typename T2>
    void move(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void move(const T1 & destination, const T2 & source);

    template <typename T1, typename T2>
    void swap(T1 & a, T2 & b);
    template <typename T1, typename T2>
    void swap(const T1 & a, T2 & b);
    template <typename T1, typename T2>
    void swap(T1 & a, const T2 & b);
    template <typename T1, typename T2>
    void swap(const T1 & a, const T2 & b);

    // Reads a character and stores it in the destination memory block
    template <typename T>
    void read(T & destination);
    template <typename T>
    void read(const T & destination);

    // Takes a pointer to an array of characters, and reads a string of data into that array. If the pointer doesn't
    // point to an array, it will read just one character into the memory that the pointer is pointing to
    template <typename T>
    void readString(T & destination);
    template <typename T>
    void readString(const T & destination);

    template <typename T>
    void write(const T & source);

    template <typename T>
    void writeString(const T & source);

    template <typename T>
    void push(const T & source);

    template <typename T>
    void pop(T & destination);
    template <typename T>
    void pop(const T & destination);

    template <typename T>
    void increment(T & destination);
    template <typename T>
    void increment(const T & destination);

    template <typename T>
    void decrement(T & destination);
    template <typename T>
    void decrement(const T & destination);

    template <typename T>
    void negate(T & destination);
    template <typename T>
    void negate(const T & destination);

    template <typename T>
    void absolute(T & destination);
    template <typename T>
    void absolute(const T & destination);

    template <typename T1, typename T2>
    void add(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void add(const T1 & destination, const T2 & source);

    template <typename T1, typename T2>
    void subtract(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void subtract(const T1 & destination, const T2 & source);

    template <typename T1, typename T2>
    void multiply(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void multiply(const T1 & destination, const T2 & source);

    template <typename T1, typename T2>
    void divide(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void divide(const T1 & destination, const T2 & source);

    template <typename T1, typename T2>
    void modulo(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void modulo(const T1 & destination, const T2 & source);

    void stackAdd();
    void stackSubtract();
    void stackMultiply();
    void stackDivide();
    void stackModulo();

    void allocateDirect(Block & pointerDestination, Block::DataType dataType, unsigned count);
    template <typename T>
    void allocate(Block::DataType dataType, const T & count);

    template <typename T>
    void startPopulatingArray(const T & arrayPointer);
    void stopPopulatingArray();
    template <typename T>
    void addToArray(const T & value);

    template <typename T>
    void getArrayElement(const T & arrayPointer, unsigned index);
    template <typename T1, typename T2>
    void getArrayElement(const T1 & arrayPointer, const T2 & index);

    template <typename T1, typename T2>
    void getArrayLength(const T1 & arrayPointer, T2 & destination);
    template <typename T1, typename T2>
    void getArrayLength(const T1 & arrayPointer, const T2 & destination);

    template <typename T1, typename T2>
    void copyArray(const T1 & destArrayPointer, const T2 & sourceArrayPointer);

    template <typename T1, typename T2>
    void convert(T1 & destination, const T2 & source, Block::DataType dataType);
    template <typename T1, typename T2>
    void convert(const T1 & destination, const T2 & source, Block::DataType dataType);

    template <typename T1, typename T2>
    void convertToDataTypeOf(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void convertToDataTypeOf(const T1 & destination, const T2 & source);

    template <typename T1, typename T2>
    void dereference(T1 & destinaton, const T2 & pointer);
    template <typename T1, typename T2>
    void dereference(const T1 & destination, const T2 & pointer);

    template <typename T1, typename T2>
    void compare(const T1 & lhs, const T2 & rhs);

    template <typename T1, typename T2>
    void compareDataType(const T1 & lhs, const T2 & rhs);

    template <typename T>
    void isDataType(const T & operand, Block::DataType dataType);

    template <typename T>
    void copyFlag(T & destination, ComparisonFlagRegister::ComparisonFlagId flagId);
    template <typename T>
    void copyFlag(const T & destination, ComparisonFlagRegister::ComparisonFlagId flagId);

    template <typename T>
    void logicalNot(T & destination);
    template <typename T>
    void logicalNot(const T & destination);

    template <typename T1, typename T2>
    void logicalAnd(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void logicalAnd(const T1 & destination, const T2 & source);

    template <typename T1, typename T2>
    void logicalOr(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void logicalOr(const T1 & destination, const T2 & source);

    template <typename T1, typename T2>
    void logicalXor(T1 & destination, const T2 & source);
    template <typename T1, typename T2>
    void logicalXor(const T1 & destination, const T2 & source);

    void jump(const std::string & labelName);
    void conditionalJump(const std::string & labelName, ComparisonFlagRegister::ComparisonFlagId condition);
    void call(const std::string & labelName);
    template<typename T>
    void returnFromCall(const T & returnValue);

    void loadExtension(const std::string & fileName);
    // returnFromCall is called in this function, so stack frame and parameters must already be pushed before calling!
    void extensionCall(const std::string & functionName);

    Stack & stack();
    Heap & unmanagedHeap();
    ManagedHeap & managedHeap();
    ComparisonFlagRegister & comparisonFlagRegister();
    Block & primaryRegister();
    Block & managedOutRegister();
    unsigned & programCounter();

    const LabelTable & labels() const;
    void addLabel(const std::string & labelName, unsigned lineNumber);

    bool & operand1IsPointer();
    bool operand1IsPointer() const;
    bool & operand2IsPointer();
    bool operand2IsPointer() const;

    Block * getBlockFrom(locationId location, short operandNumber);
    Block * getBlockFrom(StackLocation location, short operandNumber);
    Block * getBlockFrom(HeapLocation location, short operandNumber);
    Block * getBlockFrom(Block & pointer, short operandNumber);
    const Block * getBlockFrom(const Block & pointer, short operandNumber);

private:
    static std::vector<void*> extensionHandles;
    static unsigned machineCount;
    static const unsigned extensionMachineStackSize, extensionMachineHeapSize;

    Stack stack_;
    Heap unmanagedHeap_;
    ManagedHeap managedHeap_;
    ComparisonFlagRegister comparisonFlagRegister_;
    Block primaryRegister_,
    managedOutRegister_; // a register for storing the output of managed heap functions
    unsigned programCounter_;

    LabelTable labels_;
    ReturnAddressStack returnAddressStack;

    class ArrayPopulator
    {
    public:
        ArrayPopulator();
        void start(const Block & pointerToArray);
        void stop();
        void add(const Block & value);

    private:
        Block arrayPointer;
        int currentIndex;
    } arrayBeingPopulated;

    bool operand1IsPointer_, operand2IsPointer_;

    Machine * extensionMachine; // A separate machine for extension functions to work inside

    /* Functions handling the operations shared by many other functions.
     * Even if operations are trivial, these functions should be used so that the operation can be changed with ease
     * if need be. It saves time and is less prone to error.
     * Identifiers are preceded by underscores to avoid template recursion problems
     */
    void _clear(Block * locationBlock);
    void _set(Block * destBlock, const Block * value);
    void _move(Block * destBlock, const Block * sourceBlock);
    void _swap(Block * a, Block * b);
    void _read(Block * destBlock);
    void _readString(Block * destBlock);
    void _write(const Block * sourceBlock);
    void _writeString(const Block * sourceBlock);
    void _push(const Block * sourceBlock);
    void _pop(Block * destBlock);
    void _increment(Block * destBlock);
    void _decrement(Block * destBlock);
    void _negate(Block * destBlock);
    void _absolute(Block * destBlock);
    void _add(Block * destBlock, const Block * sourceBlock);
    void _subtract(Block * destBlock, const Block * sourceBlock);
    void _multiply(Block * destBlock, const Block * sourceBlock);
    void _divide(Block * destBlock, const Block * sourceBlock);
    void _modlulo(Block * destBlock, const Block * sourceBlock);
    void _allocate(const Block::DataType dataType, const Block * countBlock);
    void _startPopulatingArray(const Block * pointerBlock);
    void _addToArray(const Block * valueBlock);
    void _getArrayElement(const Block * pointerBlock, unsigned index);
    void _getArrayElement(const Block * pointerBlock, const Block * indexBlock);
    void _getArrayLength(const Block * pointerBlock, Block * destBlock);
    void _copyArray(const Block * destPointerBlock, const Block * sourcePointerBlock);
    void _convert(Block * destBlock, const Block * sourceBlock, Block::DataType dataType);
    void _convertToDataTypeOf(Block * destBlock, const Block * sourceBlock);
    void _dereference(Block * destBlock, const Block * pointerBlock);
    void _compare(const Block * lhsBlock, const Block * rhsBlock);
    void _compareDataType(const Block * lhsBlock, const Block * rhsBlock);
    void _isDataType(const Block * block, Block::DataType dataType);
    void _copyFlag(Block * destBlock, ComparisonFlagRegister::ComparisonFlagId flagId);
    void _logicalNot(Block * destBlock);
    void _logicalAnd(Block * destBlock, const Block * sourceBlock);
    void _logicalOr(Block * destBlock, const Block * sourceBlock);
    void _logicalXor(Block * destBlock, const Block * sourceBlock);
    void _returnFromCall(const Block * returnBlock);
    void _extensionCall(const std::string & functionName);
};

template <typename T>
void Machine::clear(T & location)
{
    _clear(getBlockFrom(location, 1));
}
template <typename T>
void Machine::clear(const T & location)
{
    _clear(getBlockFrom(location, 1));
}

template <typename T1, typename T2>
void Machine::set(T1 & destination, const T2 & value)
{
    _set(getBlockFrom(destination, 1), getBlockFrom(value, 2));
}
template <typename T1, typename T2>
void Machine::set(const T1 & destination, const T2 & value)
{
    _set(getBlockFrom(destination, 1), getBlockFrom(value, 2));
}

template <typename T1, typename T2>
void Machine::move(T1 & destination, const T2 & source)
{
    _move(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::move(const T1 & destination, const T2 & source)
{
    _move(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T1, typename T2>
void Machine::swap(T1 & a, T2 & b)
{
    _swap(getBlockFrom(a, 1), getBlockFrom(b, 2));
}
template <typename T1, typename T2>
void Machine::swap(const T1 & a, T2 & b)
{
    _swap(getBlockFrom(a, 1), getBlockFrom(b, 2));
}
template <typename T1, typename T2>
void Machine::swap(T1 & a, const T2 & b)
{
    _swap(getBlockFrom(a, 1), getBlockFrom(b, 2));
}
template <typename T1, typename T2>
void Machine::swap(const T1 & a, const T2 & b)
{
    _swap(getBlockFrom(a, 1), getBlockFrom(b, 2));
}

template <typename T>
void Machine::read(T & destination)
{
    _read(getBlockFrom(destination, 1));
}
template <typename T>
void Machine::read(const T & destination)
{
    _read(getBlockFrom(destination, 1));
}
template <typename T>
void Machine::readString(T & destination)
{
    _readString(getBlockFrom(destination, 1));
}
template <typename T>
void Machine::readString(const T & destination)
{
    _readString(getBlockFrom(destination, 1));
}

template <typename T>
void Machine::write(const T & source)
{
    _write(getBlockFrom(source, 1));
}

template <typename T>
void Machine::writeString(const T & source)
{
    _writeString(getBlockFrom(source, 1));
}

template <typename T>
void Machine::push(const T & source)
{
    _push(getBlockFrom(source, 1));
}

template <typename T>
void Machine::pop(T & destination)
{
    _pop(getBlockFrom(destination, 1));
}
template <typename T>
void Machine::pop(const T & destination)
{
    _pop(getBlockFrom(destination, 1));
}

template <typename T>
void Machine::increment(T & destination)
{
    _increment(getBlockFrom(destination, 1));
}
template <typename T>
void Machine::increment(const T & destination)
{
    _increment(getBlockFrom(destination, 1));
}

template <typename T>
void Machine::decrement(T & destination)
{
    _decrement(getBlockFrom(destination, 1));
}
template <typename T>
void Machine::decrement(const T & destination)
{
    _decrement(getBlockFrom(destination, 1));
}

template <typename T>
void Machine::negate(T & destination)
{
    _negate(getBlockFrom(destination, 1));
}
template <typename T>
void Machine::negate(const T & destination)
{
    _negate(getBlockFrom(destination, 1));
}

template <typename T>
void Machine::absolute(T & destination)
{
    _absolute(getBlockFrom(destination, 1));
}
template <typename T>
void Machine::absolute(const T & destination)
{
    _absolute(getBlockFrom(destination, 1));
}

template <typename T1, typename T2>
void Machine::add(T1 & destination, const T2 & source)
{
    _add(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::add(const T1 & destination, const T2 & source)
{
    _add(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T1, typename T2>
void Machine::subtract(T1 & destination, const T2 & source)
{
    _subtract(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::subtract(const T1 & destination, const T2 & source)
{
    _subtract(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T1, typename T2>
void Machine::multiply(T1 & destination, const T2 & source)
{
    _multiply(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::multiply(const T1 & destination, const T2 & source)
{
    _multiply(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T1, typename T2>
void Machine::divide(T1 & destination, const T2 & source)
{
    _divide(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::divide(const T1 & destination, const T2 & source)
{
    _divide(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T1, typename T2>
void Machine::modulo(T1 & destination, const T2 & source)
{
    _modlulo(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::modulo(const T1 & destination, const T2 & source)
{
    _modlulo(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T>
void Machine::allocate(const Block::DataType dataType, const T & count)
{
    _allocate(dataType, getBlockFrom(count, 2));
}

template <typename T>
void Machine::startPopulatingArray(const T & arrayPointer)
{
    _startPopulatingArray(getBlockFrom(arrayPointer, 1));
}

template <typename T>
void Machine::addToArray(const T & value)
{
    _addToArray(getBlockFrom(value, 1));
}

template <typename T>
void Machine::getArrayElement(const T & arrayPointer, unsigned index)
{
    _getArrayElement(getBlockFrom(arrayPointer, 1), index);
}
template <typename T1, typename T2>
void Machine::getArrayElement(const T1 & arrayPointer, const T2 & index)
{
    _getArrayElement(getBlockFrom(arrayPointer, 1), getBlockFrom(index, 2));
}

template <typename T1, typename T2>
void Machine::getArrayLength(const T1 & arrayPointer, T2 & destination)
{
    _getArrayLength(getBlockFrom(arrayPointer, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::getArrayLength(const T1 & arrayPointer, const T2 & destination)
{
    _getArrayLength(getBlockFrom(arrayPointer, 1), getBlockFrom(destination, 2));
}

template <typename T1, typename T2>
void Machine::copyArray(const T1 & destArrayPointer, const T2 & sourceArrayPointer)
{
    _copyArray(getBlockFrom(destArrayPointer, 1), getBlockFrom(sourceArrayPointer, 2));
}

template <typename T1, typename T2>
void Machine::convert(T1 & destination, const T2 & source, const Block::DataType dataType)
{
    _convert(getBlockFrom(destination, 1), getBlockFrom(source, 2), dataType);
}
template <typename T1, typename T2>
void Machine::convert(const T1 & destination, const T2 & source, const Block::DataType dataType)
{
    _convert(getBlockFrom(destination, 1), getBlockFrom(source, 2), dataType);
}

template <typename T1, typename T2>
void Machine::convertToDataTypeOf(T1 & destination, const T2 & source)
{
    _convertToDataTypeOf(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::convertToDataTypeOf(const T1 & destination, const T2 & source)
{
    _convertToDataTypeOf(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T1, typename T2>
void Machine::dereference(T1 & destination, const T2 & pointer)
{
    _dereference(getBlockFrom(destination, 1), getBlockFrom(pointer, 2));
}
template <typename T1, typename T2>
void Machine::dereference(const T1 & destination, const T2 & pointer)
{
    _dereference(getBlockFrom(destination, 1), getBlockFrom(pointer, 2));
}

template <typename T1, typename T2>
void Machine::compare(const T1 & lhs, const T2 & rhs)
{
    _compare(getBlockFrom(lhs, 1), getBlockFrom(rhs, 2));
}

template <typename T1, typename T2>
void Machine::compareDataType(const T1 & lhs, const T2 & rhs)
{
    _compareDataType(getBlockFrom(lhs, 1), getBlockFrom(rhs, 2));
}

template <typename T>
void Machine::isDataType(const T & operand, const Block::DataType dataType)
{
    _isDataType(getBlockFrom(operand, 1), dataType);
}

template <typename T>
void Machine::copyFlag(T & destination, const ComparisonFlagRegister::ComparisonFlagId flagId)
{
    _copyFlag(getBlockFrom(destination, 1), flagId);
}
template <typename T>
void Machine::copyFlag(const T & destination, const ComparisonFlagRegister::ComparisonFlagId flagId)
{
    _copyFlag(getBlockFrom(destination, 1), flagId);
}

template <typename T>
void Machine::logicalNot(T & destination)
{
    _logicalNot(getBlockFrom(destination, 1));
}
template <typename T>
void Machine::logicalNot(const T & destination)
{
    _logicalNot(getBlockFrom(destination, 1));
}

template <typename T1, typename T2>
void Machine::logicalAnd(T1 & destination, const T2 & source)
{
    _logicalAnd(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::logicalAnd(const T1 & destination, const T2 & source)
{
    _logicalAnd(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T1, typename T2>
void Machine::logicalOr(T1 & destination, const T2 & source)
{
    _logicalOr(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::logicalOr(const T1 & destination, const T2 & source)
{
    _logicalOr(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T1, typename T2>
void Machine::logicalXor(T1 & destination, const T2 & source)
{
    _logicalXor(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}
template <typename T1, typename T2>
void Machine::logicalXor(const T1 & destination, const T2 & source)
{
    _logicalXor(getBlockFrom(destination, 1), getBlockFrom(source, 2));
}

template <typename T>
void Machine::returnFromCall(const T & returnValue)
{
    _returnFromCall(getBlockFrom(returnValue, 1));
}

#endif // MACHINE_HPP
