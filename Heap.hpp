/*
 * Heap.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef HEAP_HPP
#define HEAP_HPP

#include <memory>

// The unmanaged heap

class Block;

class Heap
{
public:
    static const unsigned defaultSize;

    Heap(unsigned size);

    Block & blockAt(unsigned index); // Access by an 'address', i.e. an array index

    // References counts are simply conveniences in a regular heap.
    // In a managed heap, they are used for garbage collection
    void incReferenceCountAt(unsigned index);
    void decReferenceCountAt(unsigned index);
    unsigned short referenceCountAt(unsigned index);

private:
    unsigned size;
    std::auto_ptr<Block> data;
    std::auto_ptr<unsigned short> referenceCount;
};

#endif // HEAP_HPP
