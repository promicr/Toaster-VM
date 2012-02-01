/*
 * ToasterVM.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <exception>

#include "Flags.hpp"
#include "Stack.hpp"
#include "Heap.hpp"

int main(int argc, char * argv[])
{
    FlagRegister flags;

    Stack * stack;
    try
    {
        stack = new Stack(0);
    }
    catch (const std::exception & exception)
    {
        std::cout << exception.what() << std::endl;
        return 0;
    }

    Heap * heap;
    try
    {
        heap = new Heap(0);
    }
    catch (const std::exception & exception)
    {
        std::cout << exception.what() << std::endl;
        delete stack;
        return 0;
    }

    delete stack;
    delete heap;

    return 0;
}
