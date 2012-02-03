/*
 * Machine.hpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <map>

#include "Stack.hpp"
#include "ManagedHeap.hpp"
#include "ComparisonFlagRegister.hpp"

class Machine
{
public:
    typedef std::map<std::string, unsigned> labelTable; // Maybe change string to an int, i.e. hashed label name

    enum locationId
    {
        L_STACK,
        L_PRIMARY_REGISTER,
        L_MANAGED_OUT_REGISTER,
        L_TRASH
    };

    Machine(unsigned stackSize = 0, unsigned unmanagedHeapSize = 0, unsigned managedHeapSize = 0);

    // Member functions relating to the bytecode
    void clear(locationId location);
    void clear(Block & location);

    void set(const Block & value, locationId destination);
    void set(const Block & value, Block & destination);

    void move(locationId source, locationId destination);
    void move(locationId source, Block & destination);
    void move(const Block & source, locationId destination);
    void move(const Block & source, Block & destination);

    // Reads a character and stores it in the destination memory block. Will read in a string of characters if
    // destination is an array of characters
    void read(locationId destination);
    void read(Block & destination);

    void write(locationId source);
    void write(const Block & source);

    void push(locationId source);
    void push(const Block & source);

    void pop(locationId destination);
    void pop(Block & destination);

    void increment(locationId destination);
    void increment(Block & destination);

    void decrement(locationId destination);
    void decrement(Block & destination);

    void add(locationId source, locationId destination);
    void add(locationId source, Block & destination);
    void add(const Block & source, locationId destination);
    void add(const Block & source, Block & destination);

    void subtract(locationId source, locationId destination);
    void subtract(locationId source, Block & destination);
    void subtract(const Block & source, locationId destination);
    void subtract(const Block & source, Block & pointerToDest);

    void multiply(locationId source, locationId destination);
    void multiply(locationId source, Block & destination);
    void multiply(const Block & source, locationId destination);
    void multiply(const Block & source, Block & destination);

    void divide(locationId source, locationId destination);
    void divide(locationId source, Block & destination);
    void divide(const Block & source, locationId destination);
    void divide(const Block & source, Block & destination);

    void allocate(Block::DataType dataType, unsigned count);
    void allocate(Block::DataType dataType, locationId count);
    void allocate(Block::DataType dataType, const Block & count);

    void getArrayElement(locationId arrayPointer, unsigned index);
    void getArrayElement(locationId arrayPointer, locationId index);
    void getArrayElement(locationId arrayPointer, const Block & index);
    void getArrayElement(const Block & arrayPointer, unsigned index);
    void getArrayElement(const Block & arrayPointer, locationId index);
    void getArrayElement(const Block & arrayPointer, const Block & index);

    void getArrayLength(locationId arrayPointer, locationId destination);
    void getArrayLength(locationId arrayPointer, Block & destination);
    void getArrayLength(const Block & arrayPointer, locationId destination);
    void getArrayLength(const Block & arrayPointer, Block & destination);

    void compare(locationId lhs, locationId rhs);
    void compare(locationId lhs, const Block & rhs);
    void compare(const Block & lhs, locationId rhs);
    void compare(const Block & lhs, const Block & rhs);

    void copyFlag(ComparisonFlagRegister::ComparisonFlagId flagId, locationId destination);
    void copyFlag(ComparisonFlagRegister::ComparisonFlagId flagId, Block & destination);

    void jump(const std::string & labelName);
    void conditionalJump(ComparisonFlagRegister::ComparisonFlagId condition, const std::string & labelName);

    Stack & stack();
    Heap & unmanagedHeap();
    ManagedHeap & managedHeap();
    ComparisonFlagRegister & comparisonFlagRegister();
    Block & primaryRegister();
    Block & managedOutRegister();
    unsigned & programCounter();

    const labelTable & labels() const;
    void addLabel(const std::string & labelName, unsigned lineNumber);

    bool & operand1IsPointer();
    bool operand1IsPointer() const;
    bool & operand2IsPointer();
    bool operand2IsPointer() const;

private:
    Stack stack_;
    Heap unmanagedHeap_;
    ManagedHeap managedHeap_;
    ComparisonFlagRegister comparisonFlagRegister_;
    Block primaryRegister_,
    managedOutRegister_; // a register for storing the output of managed heap functions
    unsigned programCounter_;

    labelTable labels_;

    bool operand1IsPointer_, operand2IsPointer_;

    /* Functions handling the operations shared by many other functions.
     * Even if operations are trivial, these functions should be used so that the operation can be changed with ease
     * if need be. It saves time and is less prone to error.
     * Pointers should be validated BEFORE being passed to these functions
     */
    void clear(Block * locationBlock);
    void set(const Block * value, Block * destBlock);
    void move(const Block * sourceBlock, Block * destBlock);
    void read(Block * destBlock);
    void write(const Block * sourceBlock);
    void push(const Block * sourceBlock);
    void pop(Block * destBlock);
    void increment(Block * destBlock);
    void decrement(Block * destBlock);
    void add(const Block * sourceBlock, Block * destBlock);
    void subtract(const Block * sourceBlock, Block * destBlock);
    void multiply(const Block * sourceBlock, Block * destBlock);
    void divide(const Block * sourceBlock, Block * destBlock);
    void getArrayElement(const Block * pointerBlock, unsigned index);
    void getArrayLength(const Block * pointerBlock, Block * destBlock);
    void compare(const Block * lhsBlock, const Block * rhsBlock);

    Block * getBlockFrom(locationId location, short operandNumber);
    Block * getBlockFrom(Block & pointer, short operandNumber);
    const Block * getBlockFrom(const Block & pointer, short operandNumber);
};

#endif // MACHINE_HPP
