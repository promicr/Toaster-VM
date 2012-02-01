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
#include "Flags.hpp"

class Machine
{
public:
    enum locationId
    {
        L_STACK,
        L_UNMANAGED_HEAP,
        L_MANAGED_HEAP,
        L_PRIMARY_REGISTER,
        L_ALLOC_OUT_REGISTER,
        L_TRASH
    };

    Machine(unsigned stackSize = 0, unsigned heapSize = 0);

    // Member functions relating to the bytecode
    void clear(locationId location);
    void set(const Block & value, locationId destination);
    void move(locationId source, locationId destination);

    // Reads a character and stores it in the destination memory block. Will read in a string of characters if
    // destination is an array of characters
    void read(locationId destination);
    void write(locationId source);

    void push(locationId source);
    void pop(locationId destination);

    void add(locationId source, locationId destination);
    void subtract(locationId source, locationId destination);
    void multiply(locationId source, locationId destination);
    void divide(locationId source, locationId destination);

private:
    Stack stack;
    Heap unmanagedHeap;
    ManagedHeap managedHeap;
    FlagRegister flagRegister;
    Block primaryRegister,
    allocOutRegister; // a register for storing the pointer output of managed heap allocations

    Block * getBlockFrom(locationId location);
};

#endif // MACHINE_HPP
