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

    //Initialises the unmanaged heap with the number of Blocks specified. Returns true on success
    static bool init(unsigned size);
    static void free();

    static Block & blockAt(unsigned index); // Access by an 'address', i.e. an array index

protected:
    Heap(unsigned size);
    ~Heap();

private:
    static Heap * instance;

    Block * data;
    unsigned size;
};

#endif // HEAP_HPP
