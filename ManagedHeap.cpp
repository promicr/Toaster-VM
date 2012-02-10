/*
 * ManagedHeap.cpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#include <stdexcept>

#include "ManagedHeap.hpp"
#include "Block.hpp"

ManagedHeap::ManagedHeap(const unsigned size)
    : Heap(size), arrayLength(size == 0 ? defaultSize : size, 0) {}

void ManagedHeap::allocate(Block & pointerDestination, const Block::DataType dataType, const unsigned amount)
{
    unsigned size_ = size(), index;
    bool success = false;
    // First search for an empty space
    for (index = 0; index < size_; ++index)
    {
        if (referenceCountAt(index) == 0)
        {
            success = true;
            // When a space is found, make sure all blocks ahead that are required are free
            for (unsigned j = 1; j < amount; ++j)
            {
                if (index + j >= size_)
                {
                    success = false;
                    break;
                }
                if (referenceCountAt(index + j) != 0)
                {
                    success = false;
                    index += amount - 1; // No point looking at blocks before the used block, so jump to space after it.
                    break;               // Add amount - 1 (as apposed to just amount) to negate +1 of outer loop
                }
            }

        }
        if (success) break;
    }

    if (success)
    {
        arrayLength[index] = amount;
        for (unsigned i = 0; i < amount; ++i) blockAt(index + i).setTo(dataType);
        pointerDestination.setToPointer(index, *this);
    }
    else
    {
        pointerDestination.setToPointer();
        throw(std::runtime_error("ManagedHeap::allocate: Data could not be allocated"));
    }
}

unsigned ManagedHeap::arrayLengthAt(const unsigned index)
{
    if (index >= size()) throw(std::out_of_range("ManagedHeap::arrayLengthAt: Array length index out of range"));
    return arrayLength[index];
}

void ManagedHeap::referenceCountChangeCallback(const unsigned index)
{
    unsigned refCount = referenceCountAt(index);
    if (refCount == 0) arrayLength[index] = 0;
    // Entire array needs to have reference count updated so that ManagedHeap::allocate works simply
    for (unsigned i = 1; i < arrayLength[index]; ++i) setReferenceCountAt(index + i, refCount, false);
}
