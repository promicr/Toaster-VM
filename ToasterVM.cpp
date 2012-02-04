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
    machine.allocate(Block::DT_CHAR, 128);
    machine.readString(Machine::L_MANAGED_OUT_REGISTER);
    machine.writeString(Machine::L_MANAGED_OUT_REGISTER);

    return 0;
}
