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
    Machine machine(50, 50);

    machine.allocate(Block::DT_INTEGER, 1);
    machine.write(Machine::L_ALLOC_OUT_REGISTER);
    machine.move(Machine::L_ALLOC_OUT_REGISTER, Machine::L_PRIMARY_REGISTER);
    machine.clear(Machine::L_ALLOC_OUT_REGISTER);

    Block pointer(machine.primaryRegister());
    machine.set(Block(Block::DT_INTEGER), pointer);

    machine.clear(Machine::L_PRIMARY_REGISTER);

    machine.write(pointer);

    Block * block = &machine.managedHeap().blockAt(pointer.pointerAddress());
    std::cout << "IN USE? " << block->inUse() << std::endl << "..." << std::endl;
    pointer.setUnused();

    std::cout << "IN USE NOW? " << block->inUse() << std::endl;
    return 0;
}
