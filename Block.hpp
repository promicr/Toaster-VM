/*
 * Block.hpp
 *
 *  Created on: 31 Jan 2012
 *      Author: Max Foster
 */

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>

#include "TypeWrappers.hpp"

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

    explicit Block(Integer value);
    explicit Block(Real value);
    explicit Block(Char value);
    explicit Block(Boolean value);
    explicit Block(int address, Heap & heap);
    explicit Block(DataType dataType);

    ~Block();

    DataType dataType() const;

    long integerData() const;
    long & integerData();

    double realData() const;
    double & realData();

    char charData() const;
    char & charData();

    bool booleanData() const;
    bool & booleanData();

    int pointerAddress() const;
    int & pointerAddress();

    Heap * pointerHeap() const;
    bool pointerIsNull() const;
    unsigned pointerArrayLength() const;
    Block * pointerArrayElementAt(unsigned index) const;
    Block * pointerDataPointedTo() const;
    // Cleans the pointer data of a block, decrementing a heap reference count if necessary
    void nullifyPointerData(bool decReference = true);

    void setToInteger(long data = 0u);
    void setToReal(double data = 0.0);
    void setToChar(char data = '\0');
    void setToBoolean(bool data = false);
    void setToPointer(int address, Heap & heap);
    void setToPointer(); // set to null pointer
    void setTo(DataType dataType); // Sets the block to the specified datatype with it's default value (i.e. 0)

    void clear();

    Block & operator =(const Block & rhs);
    bool operator ==(const Block & rhs) const;
    bool operator !=(const Block & rhs) const;

private:
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
};

std::ostream & operator <<(std::ostream & stream, const Block & block);

#endif // BLOCK_HPP
