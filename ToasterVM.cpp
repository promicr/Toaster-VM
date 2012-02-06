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
    Interpreter interpreter(machine);
    interpreter.run();

    return 0;
}
