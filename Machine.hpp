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

    void set(const Block & value, locationId destination);
    void set(const Block & value, const Block & pointerToDest);

    void move(locationId source, locationId destination);
    void move(locationId source, const Block & pointerToDest);
    void move(const Block & pointerToSource, locationId destination);
    void move(const Block & pointerToSource, const Block & pointerToDest);

    // Reads a character and stores it in the destination memory block. Will read in a string of characters if
    // destination is an array of characters
    void read(locationId destination);
    void read(const Block & pointerToDest);

    void write(locationId source);
    void write(const Block & pointerToSource);

    void push(locationId source);
    void push(const Block & pointerToSource);

    void pop(locationId destination);
    void pop(const Block & pointerToDest);

    void increment(locationId destination);
    void increment(const Block & pointerToDestination);

    void decrement(locationId destination);
    void decrement(const Block & pointerToDestination);

    void add(locationId source, locationId destination);
    void add(locationId source, const Block & pointerToDest);
    void add(const Block & pointerToSource, locationId destination);
    void add(const Block & pointerToSource, const Block & pointerToDest);

    void subtract(locationId source, locationId destination);
    void subtract(locationId source, const Block & pointerToDest);
    void subtract(const Block & pointerToSource, locationId destination);
    void subtract(const Block & pointerToSource, const Block & pointerToDest);

    void multiply(locationId source, locationId destination);
    void multiply(locationId source, const Block & pointerToDest);
    void multiply(const Block & pointerToSource, locationId destination);
    void multiply(const Block & pointerToSource, const Block & pointerToDest);

    void divide(locationId source, locationId destination);
    void divide(locationId source, const Block & pointerToDest);
    void divide(const Block & pointerToSource, locationId destination);
    void divide(const Block & pointerToSource, const Block & pointerToDest);

    void allocate(Block::DataType dataType, unsigned count);

    void compare(locationId lhs, locationId rhs);
    void compare(locationId lhs, const Block & pointerToRhs);
    void compare(const Block & pointerToLhs, locationId rhs);
    void compare(const Block & pointerToLhs, const Block & pointerToRhs);

    void copyFlag(ComparisonFlagRegister::ComparisonFlagId flagId, locationId destination);
    void copyFlag(ComparisonFlagRegister::ComparisonFlagId flagId, const Block & pointerToDest);

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
    void set(const Block & value, Block * destBlock);
    void move(Block * sourceBlock, Block * destBlock);
    void read(Block * destBlock);
    void write(Block * sourceBlock);
    void push(Block * sourceBlock);
    void pop(Block * destBlock);
    void increment(Block * destBlock);
    void decrement(Block * destBlock);
    void add(Block * sourceBlock, Block * destBlock);
    void subtract(Block * sourceBlock, Block * destBlock);
    void multiply(Block * sourceBlock, Block * destBlock);
    void divide(Block * sourceBlock, Block * destBlock);
    void compare(Block * lhsBlock, Block * rhsBlock);

    Block * getBlockFrom(locationId location);
    Block * getBlockFrom(const Block & pointer);
};

#endif // MACHINE_HPP
