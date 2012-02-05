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
    typedef Block (*ExtensionFunction)(void*, Block);
    typedef std::map<std::string, ExtensionFunction> ExtensionFunctionTable;

    struct StackLocation
    {
        unsigned value;
        explicit StackLocation(const unsigned value) : value(value) {}
    };

    enum locationId
    {
        L_STACK,
        L_PRIMARY_REGISTER,
        L_MANAGED_OUT_REGISTER,
        L_EXTENSION_OUT_REGISTER,
        L_NIL
    }
    static const STACK = L_STACK, // For use in Machine member functions that take T& arguments
    PRIMARY_REGISTER = L_PRIMARY_REGISTER,
    MANAGED_OUT_REGISTER = L_MANAGED_OUT_REGISTER,
    EXTENSION_OUT_REGISTER = L_EXTENSION_OUT_REGISTER,
    NIL = L_NIL;

    Machine(unsigned stackSize = 0, unsigned unmanagedHeapSize = 0, unsigned managedHeapSize = 0);
    ~Machine();

    // Member functions relating to the bytecode
    template <typename T>
    void clear(T & location);
    template <typename T>
    void clear(const T & location);

    template <typename T1, typename T2>
    void set(const T1 & value, T2 & destination);
    template <typename T1, typename T2>
    void set(const T1 & value, const T2 & destination);

    template <typename T1, typename T2>
    void move(const T1 & source, T2 & destination);
    template <typename T1, typename T2>
    void move(const T1 & source, const T2 & destination);

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

    template <typename T1, typename T2>
    void add(const T1 & source, T2 & destination);
    template <typename T1, typename T2>
    void add(const T1 & source, const T2 & destination);

    template <typename T1, typename T2>
    void subtract(const T1 & source, T2 & destination);
    template <typename T1, typename T2>
    void subtract(const T1 & source, const T2 & destination);

    template <typename T1, typename T2>
    void multiply(const T1 & source, T2 & destination);
    template <typename T1, typename T2>
    void multiply(const T1 & source, const T2 & destination);

    template <typename T1, typename T2>
    void divide(const T1 & source, T2 & destination);
    template <typename T1, typename T2>
    void divide(const T1 & source, const T2 & destination);

    void stackAdd();
    void stackSubtract();
    void stackMultiply();
    void stackDivide();

    void allocateDirect(Block::DataType dataType, unsigned count, Block & pointerDestination);
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
    void getArrayElement(const T2 & arrayPointer, const T2 & index);

    template <typename T1, typename T2>
    void getArrayLength(const T1 & arrayPointer, T2 & destination);
    template <typename T1, typename T2>
    void getArrayLength(const T1 & arrayPointer, const T2 & destination);

    template <typename T1, typename T2>
    void convert(const T1 & source, T2 & destination, Block::DataType dataType);
    template <typename T1, typename T2>
    void convert(const T1 & source, const T2 & destination, Block::DataType dataType);

    template <typename T1, typename T2>
    void dereference(const T1 & pointer, T2 & destinaton);
    template <typename T1, typename T2>
    void dereference(const T1 & pointer, const T2 & destination);

    template <typename T1, typename T2>
    void compare(const T1 & lhs, const T2 & rhs);

    template <typename T>
    void copyFlag(ComparisonFlagRegister::ComparisonFlagId flagId, T & destination);
    template <typename T>
    void copyFlag(ComparisonFlagRegister::ComparisonFlagId flagId, const T & destination);

    template <typename T>
    void logicalNot(T & destination);
    template <typename T>
    void logicalNot(const T & destination);

    template <typename T1, typename T2>
    void logicalAnd(const T1 & source, T2 & destination);
    template <typename T1, typename T2>
    void logicalAnd(const T1 & source, const T2 & destination);

    template <typename T1, typename T2>
    void logicalOr(const T1 & source, T2 & destination);
    template <typename T1, typename T2>
    void logicalOr(const T1 & source, const T2 & destination);

    template <typename T1, typename T2>
    void logicalXor(const T1 & source, T2 & destination);
    template <typename T1, typename T2>
    void logicalXor(const T1 & source, const T2 & destination);

    void jump(const std::string & labelName);
    void conditionalJump(ComparisonFlagRegister::ComparisonFlagId condition, const std::string & labelName);
    void call(const std::string & labelName);
    template<typename T>
    void returnFromCall(const T & returnValue);

    void loadExtension(const std::string & fileName);
    template<typename T>
    void extensionCall(const std::string & functionName, const T & argument);

    Stack & stack();
    Heap & unmanagedHeap();
    ManagedHeap & managedHeap();
    ComparisonFlagRegister & comparisonFlagRegister();
    Block & primaryRegister();
    Block & managedOutRegister();
    Block & extensionOutRegister();
    unsigned & programCounter();

    const LabelTable & labels() const;
    void addLabel(const std::string & labelName, unsigned lineNumber);

    bool & operand1IsPointer();
    bool operand1IsPointer() const;
    bool & operand2IsPointer();
    bool operand2IsPointer() const;

    static const ExtensionFunctionTable & extensionFunctions();

private:
    static std::vector<void*> extensionHandles;
    static ExtensionFunctionTable extensionFunctions_;
    static unsigned machineCount;

    Stack stack_;
    Heap unmanagedHeap_;
    ManagedHeap managedHeap_;
    ComparisonFlagRegister comparisonFlagRegister_;
    Block primaryRegister_,
    managedOutRegister_, // a register for storing the output of managed heap functions
    extensionOutRegister_; // a register for storing the output of extension functions
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

    /* Functions handling the operations shared by many other functions.
     * Even if operations are trivial, these functions should be used so that the operation can be changed with ease
     * if need be. It saves time and is less prone to error.
     * Identifiers are preceded by underscores to avoid template recursion problems
     */
    void _clear(Block * locationBlock);
    void _set(const Block * value, Block * destBlock);
    void _move(const Block * sourceBlock, Block * destBlock);
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
    void _add(const Block * sourceBlock, Block * destBlock);
    void _subtract(const Block * sourceBlock, Block * destBlock);
    void _multiply(const Block * sourceBlock, Block * destBlock);
    void _divide(const Block * sourceBlock, Block * destBlock);
    void _allocate(const Block::DataType dataType, const Block * countBlock);
    void _startPopulatingArray(const Block * pointerBlock);
    void _addToArray(const Block * valueBlock);
    void _getArrayElement(const Block * pointerBlock, unsigned index);
    void _getArrayElement(const Block * pointerBlock, const Block * indexBlock);
    void _getArrayLength(const Block * pointerBlock, Block * destBlock);
    void _convert(const Block * sourceBlock, Block * destBlock, Block::DataType dataType);
    void _dereference(const Block * pointerBlock, Block * destBlock);
    void _compare(const Block * lhsBlock, const Block * rhsBlock);
    void _copyFlag(ComparisonFlagRegister::ComparisonFlagId flagId, Block * destBlock);
    void _logicalNot(Block * destBlock);
    void _logicalAnd(const Block * sourceBlock, Block * destBlock);
    void _logicalOr(const Block * sourceBlock, Block * destBlock);
    void _logicalXor(const Block * sourceBlock, Block * destBlock);
    void _returnFromCall(const Block * returnBlock);
    void _extensionCall(const std::string & functionName, const Block * argument);

    Block * getBlockFrom(locationId location, short operandNumber);
    Block * getBlockFrom(StackLocation location, short operandNumber);
    Block * getBlockFrom(Block & pointer, short operandNumber);
    const Block * getBlockFrom(const Block & pointer, short operandNumber);
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
void Machine::set(const T1 & value, T2 & destination)
{
    _set(getBlockFrom(value, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::set(const T1 & value, const T2 & destination)
{
    _set(getBlockFrom(value, 1), getBlockFrom(destination, 2));
}

template <typename T1, typename T2>
void Machine::move(const T1 & source, T2 & destination)
{
    _move(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::move(const T1 & source, const T2 & destination)
{
    _move(getBlockFrom(source, 1), getBlockFrom(destination, 2));
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

template <typename T1, typename T2>
void Machine::add(const T1 & source, T2 & destination)
{
    _add(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::add(const T1 & source, const T2 & destination)
{
    _add(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}

template <typename T1, typename T2>
void Machine::subtract(const T1 & source, T2 & destination)
{
    _subtract(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::subtract(const T1 & source, const T2 & destination)
{
    _subtract(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}

template <typename T1, typename T2>
void Machine::multiply(const T1 & source, T2 & destination)
{
    _multiply(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::multiply(const T1 & source, const T2 & destination)
{
    _multiply(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}

template <typename T1, typename T2>
void Machine::divide(const T1 & source, T2 & destination)
{
    _divide(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::divide(const T1 & source, const T2 & destination)
{
    _divide(getBlockFrom(source, 1), getBlockFrom(destination, 2));
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
void Machine::getArrayElement(const T2 & arrayPointer, const T2 & index)
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
void Machine::convert(const T1 & source, T2 & destination, const Block::DataType dataType)
{
    _convert(getBlockFrom(source, 1), getBlockFrom(destination, 2), dataType);
}
template <typename T1, typename T2>
void Machine::convert(const T1 & source, const T2 & destination, const Block::DataType dataType)
{
    _convert(getBlockFrom(source, 1), getBlockFrom(destination, 2), dataType);
}

template <typename T1, typename T2>
void Machine::dereference(const T1 & pointer, T2 & destination)
{
    _dereference(getBlockFrom(pointer, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::dereference(const T1 & pointer, const T2 & destination)
{
    _dereference(getBlockFrom(pointer, 1), getBlockFrom(destination, 2));
}

template <typename T1, typename T2>
void Machine::compare(const T1 & lhs, const T2 & rhs)
{
    _compare(getBlockFrom(lhs, 1), getBlockFrom(rhs, 2));
}

template <typename T>
void Machine::copyFlag(const ComparisonFlagRegister::ComparisonFlagId flagId, T & destination)
{
    _copyFlag(flagId, getBlockFrom(destination, 2));
}
template <typename T>
void Machine::copyFlag(const ComparisonFlagRegister::ComparisonFlagId flagId, const T & destination)
{
    _copyFlag(flagId, getBlockFrom(destination, 2));
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
void Machine::logicalAnd(const T1 & source, T2 & destination)
{
    _logicalAnd(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::logicalAnd(const T1 & source, const T2 & destination)
{
    _logicalAnd(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}

template <typename T1, typename T2>
void Machine::logicalOr(const T1 & source, T2 & destination)
{
    _logicalOr(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::logicalOr(const T1 & source, const T2 & destination)
{
    _logicalOr(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}

template <typename T1, typename T2>
void Machine::logicalXor(const T1 & source, T2 & destination)
{
    _logicalXor(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}
template <typename T1, typename T2>
void Machine::logicalXor(const T1 & source, const T2 & destination)
{
    _logicalXor(getBlockFrom(source, 1), getBlockFrom(destination, 2));
}

template <typename T>
void Machine::returnFromCall(const T & returnValue)
{
    _returnFromCall(getBlockFrom(returnValue, 1));
}

template <typename T>
void Machine::extensionCall(const std::string & functionName, const T & argument)
{
    _extensionCall(functionName, getBlockFrom(argument, 2));
}

#endif // MACHINE_HPP
