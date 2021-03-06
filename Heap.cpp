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
    : size_(size == 0 ? defaultSize : size), data(size_, Block()), referenceCount(size_, 0) {}

Block & Heap::blockAt(const unsigned index)
{
    if (index >= size_) throw(std::out_of_range("Heap::blockAt: Block index out of range"));
    return data[index];
}

unsigned Heap::size() const
{
    return size_;
}

void Heap::flush()
{
    for (unsigned i = 0; i < data.size(); ++i) data[i].nullifyPointerData();
}

void Heap::incReferenceCountAt(const unsigned index)
{
    if (index >= size_) throw(std::out_of_range("Heap::incReferenceCountAt: Reference count index out of range"));
    if (referenceCount[index] == USHRT_MAX)
        throw(std::runtime_error("Heap::incReferenceCountAt: Reference count is already at its maximum"));

    referenceCount[index] += 1;
    referenceCountChangeCallback(index);
}

void Heap::decReferenceCountAt(const unsigned index)
{
    if (index >= size_) throw(std::out_of_range("Heap::decReferenceCountAt: Reference count index out of range"));
    if (referenceCount[index] == 0)
        throw(std::runtime_error("Heap::decReferenceCountAt: Reference count is already at its minimum (i.e. 0)"));

    referenceCount[index] -= 1;
    referenceCountChangeCallback(index);
}

void Heap::setReferenceCountAt(const unsigned index, const unsigned value, bool triggerCallback)
{
    if (index >= size_) throw(std::out_of_range("Heap::setReferenceCountAt: Reference count index out of range"));
    if (value > USHRT_MAX)
        throw(std::runtime_error("Heap::setReferenceCountAt: New reference count value given is too large"));

    referenceCount[index] = value;
    if (triggerCallback) referenceCountChangeCallback(index);
}

unsigned short Heap::referenceCountAt(const unsigned index) const
{
    if (index >= size_) throw(std::out_of_range("Heap::referenceCountAt: Reference count index out of range"));
    return referenceCount[index];
}

void Heap::referenceCountChangeCallback(unsigned) {}
