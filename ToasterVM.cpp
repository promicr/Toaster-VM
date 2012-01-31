/*
 * ToasterVM.cpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#include "Flags.hpp"
#include "Block.hpp"
#include "Heap.hpp"

bool init(const unsigned heapSize)
{
    Flags::init();
    if (!Heap::init(heapSize)) return false;

    return true;
}

void free()
{
    Heap::free();
}
#include <iostream>
using namespace std;
int main(int argc, char * argv[])
{
    init(0);
    free();

    return 0;
}
