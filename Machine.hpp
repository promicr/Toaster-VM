/*
 * Machine.hpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "Stack.hpp"
#include "ManagedHeap.hpp"
#include "ComparisonFlagRegister.hpp"

class Machine
{
public:
    enum locationId
    {
        L_STACK,
        L_PRIMARY_REGISTER,
        L_ALLOC_OUT_REGISTER,
        L_TRASH,
        L_ADDR_STACK,
        L_ADDR_PRIMARY_REGISTER,
        L_ADDR_ALLOC_OUT_REGISTER
    };

    Machine(unsigned stackSize = 0, unsigned unmanagedHeapSize = 0, unsigned managedHeapSize = 0);

    // Member functions relating to the bytecode
    void clear(locationId location);
    void clear(Block & location, bool locationIsPointer);

    void set(const Block & value, locationId destination);
    void set(const Block & value, Block & destination, bool destIsPointer);

    void move(locationId source, locationId destination);
    void move(locationId source, Block & destination, bool destIsPointer);
    void move(const Block & source, bool sourceIsPointer, locationId destination);
    void move(const Block & source, bool sourceIsPointer, Block & destination, bool destIsPointer);

    // Reads a character and stores it in the destination memory block. Will read in a string of characters if
    // destination is an array of characters
    void read(locationId destination);
    void read(Block & destination, bool destIsPointer);

    void write(locationId source);
    void write(const Block & source, bool sourceIsPointer);

    void push(locationId source);
    void push(const Block & source, bool sourceIsPointer);

    void pop(locationId destination);
    void pop(Block & destination, bool destIsPointer);

    void increment(locationId destination);
    void increment(Block & destination, bool destIsPointer);

    void decrement(locationId destination);
    void decrement(Block & destination, bool destIsPointer);

    void add(locationId source, locationId destination);
    void add(locationId source, Block & destination, bool destIsPointer);
    void add(const Block & source, bool sourceIsPointer, locationId destination);
    void add(const Block & source, bool sourceIsPointer, Block & destination, bool destIsPointer);

    void subtract(locationId source, locationId destination);
    void subtract(locationId source, Block & destination, bool destIsPointer);
    void subtract(const Block & source, bool sourceIsPointer, locationId destination);
    void subtract(const Block & source, bool sourceIsPointer, Block & pointerToDest, bool destIsPointer);

    void multiply(locationId source, locationId destination);
    void multiply(locationId source, Block & destination, bool destIsPointer);
    void multiply(const Block & source, bool sourceIsPointer, locationId destination);
    void multiply(const Block & source, bool sourceIsPointer, Block & destination, bool destIsPointer);

    void divide(locationId source, locationId destination);
    void divide(locationId source, Block & destination, bool destIsPointer);
    void divide(const Block & source, bool sourceIsPointer, locationId destination);
    void divide(const Block & source, bool sourceIsPointer, Block & destination, bool destIsPointer);

    void allocate(Block::DataType dataType, unsigned count);
    void allocate(Block::DataType dataType, locationId count);
    void allocate(Block::DataType dataType, const Block & count, bool countIsPointer);

    void getArrayElement(locationId arrayPointer, unsigned index);
    void getArrayElement(locationId arrayPointer, locationId index);
    void getArrayElement(locationId arrayPointer, const Block & index, bool indexIsPointer);
    void getArrayElement(const Block & arrayPointer, bool arrayPointerIsPointer, unsigned index);
    void getArrayElement(const Block & arrayPointer, bool arrayPointerIsPointer, locationId index);
    void getArrayElement(const Block & arrayPointer, bool arrayPointerIsPointer, const Block & index,
                         bool indexIsPointer);

    void getArrayLength(locationId arrayPointer, locationId destination);
    void getArrayLength(locationId arrayPointer, Block & destination, bool destIsPointer);
    void getArrayLength(const Block & arrayPointer, bool arrayPointerIsPointer, locationId destination);
    void getArrayLength(const Block & arrayPointer, bool arrayPointerIsPointer, Block & destination, bool destIsPointer);

    void compare(locationId lhs, locationId rhs);
    void compare(locationId lhs, const Block & rhs, bool rhsIsPointer);
    void compare(const Block & lhs, bool lhsIsPointer, locationId rhs);
    void compare(const Block & lhs, bool lhsIsPointer, const Block & rhs, bool rhsIsPointer);

    void copyFlag(ComparisonFlagRegister::ComparisonFlagId flagId, locationId destination);
    void copyFlag(ComparisonFlagRegister::ComparisonFlagId flagId, Block & destination, bool destIsPointer);

    Stack & stack();
    Heap & unmanagedHeap();
    ManagedHeap & managedHeap();
    ComparisonFlagRegister & comparisonFlagRegister();
    Block & primaryRegister();
    Block & allocOutRegister();

private:
    Stack stack_;
    Heap unmanagedHeap_;
    ManagedHeap managedHeap_;
    ComparisonFlagRegister comparisonFlagRegister_;
    Block primaryRegister_,
    allocOutRegister_; // a register for storing the pointer output of managed heap allocations

    /* Functions handling the operations shared by many other functions.
     * Even if operations are trivial, these functions should be used so that the operation can be changed with ease
     * if need be. It saves time and is less prone to error.
     * Pointers should be validated BEFORE being passed to these functions
     */
    void clear(Block * locationBlock);
    void set(const Block & value, Block * destBlock);
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

    Block * getBlockFrom(locationId location);
    Block * getBlockFrom(const Block & pointer);
};

#endif // MACHINE_HPP
