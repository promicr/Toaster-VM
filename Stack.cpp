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

Stack::Stack(const unsigned size_)
    : size(size_ == 0 ? defaultSize : size_), pointer(0)
{
    data = new Block[size];
}

Stack::~Stack()
{
    delete data;
}

void Stack::push(const Block & data_)
{
    if (pointer >= size) throw(std::runtime_error("Stack overflow"));
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
