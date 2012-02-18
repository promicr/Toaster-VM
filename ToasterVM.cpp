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

void parseCommandLineArguments(int argc, char * argv[], std::vector<Interpreter::Option> & options, char ** fileName);

int main(int argc, char * argv[])
{
    Machine machine;

    char * fileName = NULL;
    std::vector<Interpreter::Option> options;
    parseCommandLineArguments(argc, argv, options, &fileName);

    if (fileName != NULL)
    {
        Interpreter interpreter(machine, fileName, options.size(), options.data());
        interpreter.run();
    }
    else
    {
        Interpreter interpreter(machine, options.size(), options.data());
        interpreter.run();
    }

    return 0;
}

void addOption(std::vector<Interpreter::Option> & options, const char option)
{
    switch (option)
    {
    case 't': options.push_back(Interpreter::O_TIME_EXECUTION); break;
    default: break;
    }
}

void addOption(std::vector<Interpreter::Option> & options, const char * option)
{
    if (strcmp(option, "time") == 0) options.push_back(Interpreter::O_TIME_EXECUTION);
}

void parseCommandLineArguments(int argc, char * argv[], std::vector<Interpreter::Option> & options, char ** fileName)
{
    if (argc > 1) options.reserve(argc - 1); // -1 because the first option is always the path of the executable
    for (int i = 1; i < argc; ++i)
    {
        if (strstr(argv[i], "--") == argv[i]) addOption(options, argv[i][2]);
        else if (strstr(argv[i], "-") == argv[i])
        {
            for (int j = 1; argv[i][j] != '\0'; ++j) addOption(options, argv[i][j]);
        }
        else *fileName = argv[i];
    }
}
