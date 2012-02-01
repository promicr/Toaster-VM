/*
 * Machine.cpp
 *
 *  Created on: 1 Feb 2012
 *      Author: Max Foster
 */

#include <cstdio>

#include "Machine.hpp"

Machine::Machine(const unsigned stackSize, const unsigned heapSize)
    : stack(stackSize), unmanagedHeap(heapSize), managedHeap(heapSize) {}

void Machine::clear(const locationId location)
{
    Block * block = getBlockFrom(location);
    if (block != NULL) block->setUnused();
}

void Machine::set(const Block & value, const locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL) *block = value;
}

void Machine::move(const locationId source, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;

    *destBlock = *sourceBlock;
}

void Machine::read(const locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL)
    {
        char c;
        if (scanf("%c", &c) != 0) block->setToChar(c);
    }
}

void Machine::write(const locationId source)
{
    Block * block = getBlockFrom(source);
    if (block != NULL) std::cout << *block << std::endl;
}

void Machine::push(const locationId source)
{
    Block * block = getBlockFrom(source);
    if (block != NULL) stack.push(*block);
}

void Machine::pop(const locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL)
    {
        *block = stack.peek();
        stack.pop();
    }
}

void Machine::add(const locationId source, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;

    if (sourceBlock->dataType() != destBlock->dataType()) return;

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() += sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() += sourceBlock->realData();
        break;

    default: break;
    }
}

void Machine::subtract(const locationId source, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;

    if (sourceBlock->dataType() != destBlock->dataType()) return;

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() -= sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() -= sourceBlock->realData();
        break;

    default: break;
    }
}

void Machine::multiply(const locationId source, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;

    if (sourceBlock->dataType() != destBlock->dataType()) return;

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() *= sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() *= sourceBlock->realData();
        break;

    default: break;
    }
}

void Machine::divide(const locationId source, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;

    if (sourceBlock->dataType() != destBlock->dataType()) return;

    switch (sourceBlock->dataType())
    {
    case Block::DT_INTEGER:
        destBlock->integerData() /= sourceBlock->integerData();
        break;

    case Block::DT_REAL:
        destBlock->realData() /= sourceBlock->realData();
        break;

    default: break;
    }
}

Block * Machine::getBlockFrom(const locationId location)
{
    switch (location)
    {
    case L_STACK:              return &stack.peek();
    case L_PRIMARY_REGISTER:   return &primaryRegister;
    case L_ALLOC_OUT_REGISTER: return &allocOutRegister;
    default:                   return NULL;
    }
}
