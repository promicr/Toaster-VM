/*
 * Stack.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef STACK_HPP
#define STACK_HPP

#include <vector>

class Block;

class Stack
{
public:
    static const unsigned defaultSize;

    Stack(unsigned size);

    void push(const Block & data);
    void pop();

    Block & peek();
    const Block & peek() const;
    Block & at(unsigned index);
    const Block & at(unsigned index) const;
    Block & fromTop(unsigned index);
    const Block & fromTop(unsigned index) const;
    Block & fromTopBelow(unsigned index); // From the top of the stack under the stack pointer
    const Block & fromTopBelow(unsigned index) const;

    void pushFrame();
    void popFrame(const Block * returnValue);

    bool empty() const;
    unsigned count() const;
    unsigned size() const;

    void flush();

private:
    unsigned size_, pointer, combinedFramePointer;
    std::vector<Block> data;
    std::vector<unsigned> framePointerStack;
};

#endif // STACK_HPP
