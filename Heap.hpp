/*
 * Heap.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>

// The unmanaged heap

class Block;

class Heap
{
public:
    static const unsigned defaultSize;

    Heap(unsigned size);

    Block & blockAt(unsigned index); // Access by an 'address', i.e. an array index
    unsigned size() const;

protected:
    // References counts are simply conveniences in a regular heap.
    // In a managed heap, they are used for garbage collection
    void incReferenceCountAt(unsigned index);
    void decReferenceCountAt(unsigned index);
    unsigned short referenceCountAt(unsigned index) const;
    virtual void referenceCountChangeCallback(unsigned index);

    friend class Block;

private:
    unsigned size_;
    std::vector<Block> data;
    std::vector<unsigned short> referenceCount;
};

#endif // HEAP_HPP
