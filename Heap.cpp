/*
 * Heap.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <stdexcept>
#include <limits.h>

#include "Heap.hpp"
#include "Block.hpp"

const unsigned Heap::defaultSize = USHRT_MAX + 1;

Heap::Heap(const unsigned size_)
    : size(size_ == 0 ? defaultSize : size_)
{
    data = new Block[size];
}

Heap::~Heap()
{
    delete data;
}

Block & Heap::blockAt(const unsigned index)
{
    if (instance == NULL) throw(std::runtime_error("Heap not initialised"));
    if (index >= instance->size) throw(std::out_of_range("Block index out of range"));
    return instance->data[index];
}
