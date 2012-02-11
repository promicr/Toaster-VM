/*
 * ExtensionFunction.cpp
 *
 *  Created on: 6 Feb 2012
 *      Author: Max Foster
 */

#include <stdexcept>

#include "ExtensionFunction.hpp"
#include "Machine.hpp"
#include "Block.hpp"
#include "Stack.hpp"

std::vector<ExtensionFunction> ExtensionFunction::instances;

void ExtensionFunction::addNew(const char * name, const Pointer function, const unsigned parameterCount)
{
    if (parameterCount > 10)
        throw(std::runtime_error("ExtensionFunction::addNew: Only a maximum of 10 parameters allowed"));
    instances.push_back(ExtensionFunction(name, function, parameterCount));
}

ExtensionFunction * ExtensionFunction::find(const char * name)
{
    for (unsigned i = 0; i < instances.size(); ++i)
    {
        if (strcmp(instances[i].name_.c_str(), name) == 0) return &instances[i];
    }
    return NULL;
}

const std::string & ExtensionFunction::name() const
{
    return name_;
}

const ExtensionFunction::Pointer & ExtensionFunction::pointer() const
{
    return pointer_;
}

unsigned ExtensionFunction::parameterCount() const
{
    return parameterCount_;
}

Block ExtensionFunction::call(const Stack & stack, Machine * const machine) const
{
    if (stack.count() < parameterCount_)
        throw(std::runtime_error("ExtensionFunction::call: Not enough arguments on the stack"));

    // REALLY UGLY but it's the best way I could think of to make extension programming as elegant as possible
    switch (parameterCount_)
    {
    case 0:
        return pointer_(machine);
    case 1:
        return ((P1)pointer_)(machine, stack.fromTop(0));
    case 2:
        return ((P2)pointer_)(machine, stack.fromTop(1), stack.fromTop(0));
    case 3:
        return ((P3)pointer_)(machine, stack.fromTop(2), stack.fromTop(1), stack.fromTop(0));
    case 4:
        return ((P4)pointer_)(machine, stack.fromTop(3), stack.fromTop(2), stack.fromTop(1), stack.fromTop(0));
    case 5:
        return ((P5)pointer_)(machine, stack.fromTop(4), stack.fromTop(3), stack.fromTop(2), stack.fromTop(1),
                             stack.fromTop(0));
    case 6:
        return ((P6)pointer_)(machine, stack.fromTop(5), stack.fromTop(4), stack.fromTop(3), stack.fromTop(2),
                              stack.fromTop(1), stack.fromTop(0));
    case 7:
        return ((P7)pointer_)(machine, stack.fromTop(6), stack.fromTop(5), stack.fromTop(4), stack.fromTop(3),
                              stack.fromTop(2), stack.fromTop(1), stack.fromTop(0));
    case 8:
        return ((P8)pointer_)(machine, stack.fromTop(7), stack.fromTop(6), stack.fromTop(5), stack.fromTop(4),
                              stack.fromTop(3), stack.fromTop(2), stack.fromTop(1), stack.fromTop(0));
    case 9:
        return ((P9)pointer_)(machine, stack.fromTop(8), stack.fromTop(7), stack.fromTop(6), stack.fromTop(5),
                              stack.fromTop(4), stack.fromTop(3), stack.fromTop(2), stack.fromTop(1), stack.fromTop(0));
    case 10:
        return ((P10)pointer_)(machine, stack.fromTop(9), stack.fromTop(8), stack.fromTop(7), stack.fromTop(6),
                               stack.fromTop(5), stack.fromTop(4), stack.fromTop(3), stack.fromTop(2), stack.fromTop(1),
                               stack.fromTop(0));
    default: throw(std::runtime_error("ExtensionFunction::call: Only a maximum of 10 parameters allowed"));
    }
}

ExtensionFunction::ExtensionFunction(const std::string & name, const Pointer function, const unsigned parameterCount)
    : name_(name), pointer_(function), parameterCount_(parameterCount) {}
