/*
 * Stack.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef STACK_HPP
#define STACK_HPP

#include <vector>

#include "Block.hpp"

class Stack
{
public:
    static const unsigned defaultSize;

    Stack(unsigned size);

    void push(const Block &data);
    void pop();

    Block & peek();
    Block & at(unsigned index);
    Block & fromTop(unsigned index);

    unsigned size() const;

private:
    unsigned size_, pointer;
    std::vector<Block> data;
};

#endif // STACK_HPP
