/*
 * Block.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef BLOCK_HPP
#define BLOCK_HPP

// A class for a single block of data in memory

class Heap;

class Block
{
public:
    enum DataType
    {
        DT_INTEGER,
        DT_REAL,
        DT_CHAR,
        DT_BOOLEAN,
        DT_POINTER,
        DATA_TYPE_COUNT
    };

    Block();
    Block(const Block & other);

    bool inUse();
    DataType dataType();
    long integerData();
    double realData();
    char charData();
    bool booleanData();
    int pointerAddress();
    Heap * pointerHeap();

    void setUnused();
    void setToInteger(long data);
    void setToReal(double data);
    void setToChar(char data);
    void setToBoolean(bool data);
    void setToPointer(int address, Heap * heap);

    Block & operator =(const Block & rhs);
    bool operator ==(const Block & rhs);
    bool operator !=(const Block & rhs);

private:
    bool inUse_;
    DataType dataType_;

    union
    {
        long integerData_;
        double realData_;
        char charData_;
        bool booleanData_;

        struct // pointer struct
        {
            int address; // The array index of a block in one of the heaps
            Heap * heap; // The heap that the address is associated with
        } pointerData;
    };

    void cleanPointer(); // Cleans the pointer data of a block, decrementing a heap reference count if necessary
};

#endif // BLOCK_HPP
