/*
 * ToasterVM.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <exception>

#include "Machine.hpp"

int main(int argc, char * argv[])
{
    Machine machine;

    Block a, b;
    a.setToInteger(10u);
    b.setToInteger(15u);

    machine.set(a, Machine::L_PRIMARY_REGISTER);
    machine.push(Machine::L_PRIMARY_REGISTER);
    machine.set(b, Machine::L_PRIMARY_REGISTER);
    machine.add(Machine::L_PRIMARY_REGISTER, Machine::L_STACK);

    machine.write(Machine::L_PRIMARY_REGISTER);
    machine.write(Machine::L_STACK);

    machine.push(Machine::L_PRIMARY_REGISTER);
    machine.set(a, Machine::L_PRIMARY_REGISTER);
    machine.multiply(Machine::L_PRIMARY_REGISTER, Machine::L_STACK);
    machine.pop(Machine::L_PRIMARY_REGISTER);

    machine.write(Machine::L_PRIMARY_REGISTER);
    machine.write(Machine::L_STACK);

    machine.read(Machine::L_PRIMARY_REGISTER);
    machine.write(Machine::L_PRIMARY_REGISTER);

    return 0;
}
