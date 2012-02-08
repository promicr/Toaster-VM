/*
 * ToasterVM.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <exception>

#include "Machine.hpp"
#include "Interpreter.hpp"

int main(int argc, char * argv[])
{
    Machine machine;

    if (argc > 1)
    {
        Interpreter interpreter(machine, argv[1]);
        interpreter.run();
    }
    else
    {
        Interpreter interpreter(machine);
        interpreter.run();
    }

    return 0;
}
