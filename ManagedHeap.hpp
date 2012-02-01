/*
 * ManagedHeap.hpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#ifndef MANAGEDHEAP_HPP
#define MANAGEDHEAP_HPP

#include "Heap.hpp"
#include "Block.hpp"

// A heap with memory allocation functions and garbage collection. Accessed with pointer type Blocks rather than
// directly, although direct access is possible

class ManagedHeap : public Heap
{
public:
    ManagedHeap(unsigned size);

    void allocate(Block::DataType dataType, unsigned amount, Block & pointerDestination);

protected:
    void referenceCountChangeCallback(unsigned index);
};

#endif // MANAGEDHEAP_HPP
