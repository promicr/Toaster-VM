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
    : Heap(size), arraySize(size, 0) {}

void ManagedHeap::referenceCountChangeCallback(const unsigned index)
{
    if (referenceCountAt(index) == 0)
    {
        for (unsigned i = 0; i < arraySize[index]; ++i) blockAt(index + i).setUnused(false);
        arraySize[index] = 0;
    }
}

void ManagedHeap::allocate(const Block::DataType dataType, const unsigned amount, Block & pointerDestination)
{
    unsigned size_ = size(), index;
    bool success = false;
    // First search for an empty space
    for (index = 0; index < size_; ++index)
    {
        if (!blockAt(index).inUse())
        {
            success = true;
            // When a space is found, make sure all blocks ahead that are required are free
            for (unsigned j = 1; j < amount; ++j)
            {
                if (blockAt(index + j).inUse())
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
        arraySize[index] = amount;
        for (unsigned i = 0; i < amount; ++i) blockAt(index + i).setTo(dataType);
        pointerDestination.setToPointer(index, *this);
    }
    else
    {
        pointerDestination.setToPointer();
    }
}
