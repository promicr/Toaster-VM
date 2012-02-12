/*
 * ToasterVM.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <exception>
#include <cstring>

#include "Machine.hpp"
#include "Interpreter.hpp"

int main(int argc, char * argv[])
{
    Machine machine;

    char * fileName = NULL;
    bool timeExecution = false;
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-t") == 0)
        {
            timeExecution = true;
            continue;
        }
        if ((strstr(argv[i], "-") != argv[i]) && (strstr(argv[i], "--") != argv[i])) // if it isn't a CLI option
        {
            fileName = argv[i];
            continue;
        }
    }

    if (fileName != NULL)
    {
        Interpreter interpreter(machine, fileName);
        interpreter.run(timeExecution);
    }
    else
    {
        Interpreter interpreter(machine);
        interpreter.run(timeExecution);
    }

    return 0;
}
