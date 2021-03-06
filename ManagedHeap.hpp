/*
 * ManagedHeap.hpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#ifndef MANAGEDHEAP_HPP
#define MANAGEDHEAP_HPP

#include <vector>

#include "Heap.hpp"
#include "Block.hpp"

// A heap with memory allocation functions and garbage collection. Accessed with pointer type Blocks rather than
// directly, although direct access is possible

class ManagedHeap : public Heap
{
public:
    ManagedHeap(unsigned size);

    void allocate(Block & pointerDestination, Block::DataType dataType, unsigned amount);
    unsigned arrayLengthAt(unsigned index);

protected:
    void referenceCountChangeCallback(unsigned index);

private:
    std::vector<unsigned> arrayLength; // The sizes of each array of data allocated
};

#endif // MANAGEDHEAP_HPP
