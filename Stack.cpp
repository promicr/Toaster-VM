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
    : size_(size == 0 ? defaultSize : size), pointer(0), combinedFramePointer(0), data(size_, Block())
{
    framePointerStack.reserve(size_ / 4); // Just an arbitrary value really
}

void Stack::push(const Block & data_)
{
    if (combinedFramePointer + pointer >= size_) throw(std::runtime_error("Stack::push: Stack overflow"));
    data[combinedFramePointer + pointer] = data_;
    ++pointer;
}

void Stack::pop()
{
    if (pointer == 0) throw(std::runtime_error("Stack::pop: Stack underflow"));
    --pointer;
}

Block & Stack::peek()
{
    if (pointer == 0) throw(std::runtime_error("Stack::peek: No items on the stack"));
    return data[combinedFramePointer + pointer - 1];
}

const Block & Stack::peek() const
{
    if (pointer == 0) throw(std::runtime_error("Stack::peek: No items on the stack"));
    return data[combinedFramePointer + pointer - 1];
}

Block & Stack::at(const unsigned index)
{
    if (index >= pointer) throw(std::out_of_range("Stack::at: Stack block index out of range"));
    return data[combinedFramePointer + index];
}

const Block & Stack::at(const unsigned index) const
{
    if (index >= pointer) throw(std::out_of_range("Stack::at: Stack block index out of range"));
    return data[combinedFramePointer + index];
}

Block & Stack::fromTop(const unsigned index)
{
    if (index >= pointer) throw(std::out_of_range("Stack::fromTop: Stack block index out of range"));
    return data[combinedFramePointer + pointer - 1 - index];
}

const Block & Stack::fromTop(unsigned index) const
{
    if (index >= pointer) throw(std::out_of_range("Stack::fromTop: Stack block index out of range"));
    return data[combinedFramePointer + pointer - 1 - index];
}

void Stack::pushFrame()
{
    combinedFramePointer += ++pointer; // reserve a space for return value
    framePointerStack.push_back(pointer);
    pointer = 0;
}

void Stack::popFrame(const Block * returnValue)
{
    if (framePointerStack.size() == 0) throw(std::runtime_error("Stack::popFrame: Stack frame underflow"));

    unsigned oldPointer = pointer;
    pointer = framePointerStack.back();
    framePointerStack.pop_back();

    for ( ; oldPointer > pointer; --oldPointer) data[combinedFramePointer + oldPointer].clear();

    if (returnValue == NULL) --pointer, --combinedFramePointer;
    else data[combinedFramePointer - 1] = *returnValue;
    combinedFramePointer -= pointer;
}

bool Stack::empty() const
{
    return pointer == 0;
}

unsigned Stack::count() const
{
    return pointer;
}

unsigned Stack::size() const
{
    return size_;
}
