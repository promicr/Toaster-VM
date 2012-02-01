/*
 * Heap.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include <stdexcept>
#include <limits.h>

#include "Heap.hpp"
#include "Block.hpp"

const unsigned Heap::defaultSize = USHRT_MAX + 1;

Heap::Heap(const unsigned size_)
    : size(size_ == 0 ? defaultSize : size_), data(new Block[size]), referenceCount(new unsigned short[size]) {}

Block & Heap::blockAt(const unsigned index)
{
    if (index >= size) throw(std::out_of_range("Block index out of range"));
    return data.get()[index];
}

void Heap::incReferenceCountAt(unsigned index)
{
    if (index >= size) throw(std::out_of_range("Reference count index out of range"));
    if (referenceCount.get()[index] == USHRT_MAX)
        throw(std::runtime_error("Reference count is already at its maximum"));
    referenceCount.get()[index] += 1;
}

void Heap::decReferenceCountAt(unsigned index)
{
    if (index >= size) throw(std::out_of_range("Reference count index out of range"));
    if (referenceCount.get()[index] == 0)
        throw(std::runtime_error("Reference count is already at its minimum (i.e. 0)"));
    referenceCount.get()[index] -= 1;
}

unsigned short Heap::referenceCountAt(unsigned index)
{
    if (index >= size) throw(std::out_of_range("Reference count index out of range"));
    return referenceCount.get()[index];
}
