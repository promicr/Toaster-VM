/*
 * Heap.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef HEAP_HPP
#define HEAP_HPP

// The unmanaged heap

struct Block;

class Heap
{
public:
    static const unsigned defaultSize;

    Heap(unsigned size);
    ~Heap();

    Block & blockAt(unsigned index); // Access by an 'address', i.e. an array index

private:
    Block * data;
    unsigned size;
};

#endif // HEAP_HPP
