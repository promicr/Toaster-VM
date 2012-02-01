/*
 * Stack.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef STACK_HPP
#define STACK_HPP

#include <memory>

class Block;

class Stack
{
public:
    static const unsigned defaultSize;

    Stack(unsigned size);

    void push(const Block &data);
    void pop();
    Block & peek();

private:
    unsigned size, pointer;
    std::auto_ptr<Block> data;
};

#endif // STACK_HPP
