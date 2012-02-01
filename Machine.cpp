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
    if (block != NULL) set(value, block);
}

void Machine::set(const Block & value, const Block & pointerToDest)
{
    Block * block = getBlockFrom(pointerToDest);
    if (block != NULL) set(value, block);
}

void Machine::set(const Block & value, Block * destBlock)
{
    *destBlock = value;
}

void Machine::move(const locationId source, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    move(sourceBlock, destBlock);
}

void Machine::move(const locationId source, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;
    move(sourceBlock, destBlock);
}

void Machine::move(const Block & pointerToSource, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    move(sourceBlock, destBlock);
}

void Machine::move(const Block & pointerToSource, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;

    move(sourceBlock, destBlock);
}

void Machine::move(Block * sourceBlock, Block * destBlock)
{
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

void Machine::read(const Block & pointerToDest)
{
    Block * block = getBlockFrom(pointerToDest);
    if (block != NULL) read(block);
}

void Machine::read(Block * destBlock)
{
    char c;
    if (scanf("%c", &c) != 0) destBlock->setToChar(c);
}

void Machine::write(const locationId source)
{
    Block * block = getBlockFrom(source);
    if (block != NULL) write(block);
}

void Machine::write(const Block & pointerToSource)
{
    Block * block = getBlockFrom(pointerToSource);
    if (block != NULL) write(block);
}

void Machine::write(Block * sourceBlock)
{
    std::cout << *sourceBlock << std::endl;
}

void Machine::push(const locationId source)
{
    Block * block = getBlockFrom(source);
    if (block != NULL) push(block);
}

void Machine::push(const Block & pointerToSource)
{
    Block * block = getBlockFrom(pointerToSource);
    if (block != NULL) push(block);
}

void Machine::push(Block * sourceBlock)
{
    stack.push(*sourceBlock);
}

void Machine::pop(const locationId destination)
{
    Block * block = getBlockFrom(destination);
    if (block != NULL) pop(block);
}

void Machine::pop(const Block & pointerToDest)
{
    Block * block = getBlockFrom(pointerToDest);
    if (block != NULL) pop(block);
}

void Machine::pop(Block * destBlock)
{
    *destBlock = stack.peek();
    stack.pop();
}

void Machine::add(const locationId source, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const locationId source, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const Block & pointerToSource, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(const Block & pointerToSource, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;
    add(sourceBlock, destBlock);
}

void Machine::add(Block * sourceBlock, Block * destBlock)
{
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
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const locationId source, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const Block & pointerToSource, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(const Block & pointerToSource, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;
    subtract(sourceBlock, destBlock);
}

void Machine::subtract(Block * sourceBlock, Block * destBlock)
{
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
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const locationId source, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const Block & pointerToSource, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(const Block & pointerToSource, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;
    multiply(sourceBlock, destBlock);
}

void Machine::multiply(Block * sourceBlock, Block * destBlock)
{
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
    divide(sourceBlock, destBlock);
}

void Machine::divide(const locationId source, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(source);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(const Block & pointerToSource, const locationId destination)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(destination);
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(const Block & pointerToSource, const Block & pointerToDest)
{
    Block * sourceBlock = getBlockFrom(pointerToSource);
    if (sourceBlock == NULL) return;
    Block * destBlock = getBlockFrom(pointerToDest);
    if (destBlock == NULL) return;
    divide(sourceBlock, destBlock);
}

void Machine::divide(Block * sourceBlock, Block * destBlock)
{
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

Block * Machine::getBlockFrom(const Block & pointer)
{
    if (pointer.dataType() != Block::DT_POINTER) return NULL;
    return &pointer.pointerHeap().blockAt(pointer.pointerAddress());
}
