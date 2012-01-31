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
Heap * Heap::instance = NULL;

bool Heap::init(const unsigned size)
{
    if (instance != NULL) throw(std::runtime_error("Heap already initialised"));

    try
    {
        instance = new Heap(size == 0 ? defaultSize : size);
    }
    catch (const std::exception & exception)
    {
        std::cout << exception.what() << std::endl;
        instance = NULL; // just in case
    }

    return instance != NULL;
}

void Heap::free()
{
    if (instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}

Block & Heap::blockAt(const unsigned index)
{
    if (instance == NULL) throw(std::runtime_error("Heap not initialised"));
    if (index >= instance->size) throw(std::out_of_range("Block index out of range"));
    return instance->data[index];
}

Heap::Heap(const unsigned size)
    : size(size)
{
    data = new Block[size];
}

Heap::~Heap()
{
    delete data;
}
