/*
 * Stack.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <exception>
#include <stdexcept>
#include <limits.h>

#include "Stack.hpp"
#include "Block.hpp"

const unsigned Stack::defaultSize = USHRT_MAX + 1;

Stack::Stack(const unsigned size)
    : size_(size == 0 ? defaultSize : size), pointer(0), data(size_, Block()) {}

void Stack::push(const Block & data_)
{
    if (pointer >= size_) throw(std::runtime_error("Stack overflow"));
    data[pointer] = data_;
    ++pointer;
}

void Stack::pop()
{
    if (pointer == 0) throw(std::runtime_error("Stack underflow"));
    --pointer;
}

Block & Stack::peek()
{
    if (pointer == 0) throw(std::runtime_error("No items on the stack"));
    return data[pointer - 1];
}

unsigned Stack::size() const
{
    return size_;
}
