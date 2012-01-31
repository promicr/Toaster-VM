/*
 * Block.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef BLOCK_HPP
#define BLOCK_HPP

// A struct for a single block of data in memory

struct Block
{
    enum DataType
    {
        DT_NULL,
        DT_INTEGER,
        DT_REAL,
        DT_CHAR,
        DT_BOOLEAN,
        DT_POINTER,
        DATA_TYPE_COUNT
    } dataType;

    union
    {
        long integerData;
        double realData;
        char charData;
        bool booleanData;

        struct // pointer struct
        {
            struct // address struct
            {
                int value; // The array index of a block in one of the heaps
                bool inManagedHeap; // Need to differentiate between two heaps
            } address;

            DataType dataType; // Datatype of the block being pointed to
            int elementCount; // 1 for a pointer to single block of data, or > 1 for arrays

        } pointerData;

    } data;

    Block();

    void setToNull();
    void setToInteger(long data);
    void setToReal(double data);
    void setToChar(char data);
    void setToBoolean(bool data);
    void setToPointer(int address, bool inManagedHeap, DataType dataType, int elementCount);
};

#endif // BLOCK_HPP
