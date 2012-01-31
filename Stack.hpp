/*
 * Stack.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef STACK_HPP
#define STACK_HPP

struct Block;

class Stack
{
public:
    static const unsigned defaultSize;

    Stack(unsigned size);
    ~Stack();

    void push(const Block &data);
    void pop();
    Block & peek();

private:
    static Stack * instance;

    unsigned size, pointer;
    Block * data;
};

#endif // STACK_HPP
