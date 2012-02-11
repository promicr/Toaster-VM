/*
 * ExtensionFunction.hpp
 *
 *  Created on: 6 Feb 2012
 *      Author: Max Foster
 */

#ifndef EXTENSIONFUNCTION_HPP
#define EXTENSIONFUNCTION_HPP

#include <vector>

class Block;
class Stack;
class Machine;

class ExtensionFunction
{
public:
    typedef Block (*Pointer)(Machine* const); // i.e. ExtensionFunction::Pointer

    static void addNew(const char * name, Pointer function, unsigned parameterCount);
    static ExtensionFunction * find(const char * name);

    const std::string & name() const;
    const Pointer & pointer() const;
    unsigned parameterCount() const;

    Block call(const Stack & stack, Machine * const machine) const; // stack contains arguments to pass

private:
    static std::vector<ExtensionFunction> instances; // Change to something quicker to search later

    ExtensionFunction(const std::string & name, Pointer function, unsigned parameterCount);

    std::string name_;
    Pointer pointer_;
    unsigned parameterCount_;

    // Nobody really wants to see this
    typedef Block B;
    typedef B (*P1)(Machine* const, B);
    typedef B (*P2)(Machine* const, B, B);
    typedef B (*P3)(Machine* const, B, B, B);
    typedef B (*P4)(Machine* const, B, B, B, B);
    typedef B (*P5)(Machine* const, B, B, B, B, B);
    typedef B (*P6)(Machine* const, B, B, B, B, B, B);
    typedef B (*P7)(Machine* const, B, B, B, B, B, B, B);
    typedef B (*P8)(Machine* const, B, B, B, B, B, B, B, B);
    typedef B (*P9)(Machine* const, B, B, B, B, B, B, B, B, B);
    typedef B(*P10)(Machine* const, B, B, B, B, B, B, B, B, B, B);
};

#endif // EXTENSIONFUNCTION_HPP
