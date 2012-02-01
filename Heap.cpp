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

Heap::Heap(const unsigned size)
    : size_(size == 0 ? defaultSize : size), data(new Block[size_]), referenceCount(new unsigned short[size_]) {}

Block & Heap::blockAt(const unsigned index)
{
    if (index >= size_) throw(std::out_of_range("Block index out of range"));
    return data.get()[index];
}

unsigned Heap::size()
{
    return size_;
}

void Heap::incReferenceCountAt(unsigned index)
{
    if (index >= size_) throw(std::out_of_range("Reference count index out of range"));
    if (referenceCount.get()[index] == USHRT_MAX)
        throw(std::runtime_error("Reference count is already at its maximum"));

    referenceCount.get()[index] += 1;
    referenceCountChangeCallback(index);
}

void Heap::decReferenceCountAt(unsigned index)
{
    if (index >= size_) throw(std::out_of_range("Reference count index out of range"));
    if (referenceCount.get()[index] == 0)
        throw(std::runtime_error("Reference count is already at its minimum (i.e. 0)"));

    referenceCount.get()[index] -= 1;
    referenceCountChangeCallback(index);
}

unsigned short Heap::referenceCountAt(unsigned index)
{
    if (index >= size_) throw(std::out_of_range("Reference count index out of range"));
    return referenceCount.get()[index];
}

void Heap::referenceCountChangeCallback(unsigned index) {}
