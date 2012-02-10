/*
 * Opcodes.hpp
 *
 *  Created on: 10 Feb 2012
 *      Author: Max Foster
 */

#ifndef OPCODES_HPP
#define OPCODES_HPP

#include <iostream>

namespace Opcodes
{

//  1      2      3       4       5
const std::string opcodeStrings[] =
{ "clr", "set", "move", "swap", "in",    //  1
  "ins", "out", "outs", "push", "pop",   //  2
  "inc", "dec", "neg",  "abs",  "add",   //  3
  "sub", "mul", "div",  "mod",  "sadd",  //  4
  "ssub","smul","sdiv", "smod", "allc",  //  5
  "pla", "fna", "atoa", "ael",  "alen",  //  6
  "cpya","cnvi","cnvr", "cnvc", "cnvb",  //  7
  "cnvt","dref","cmp",  "cmpt", "ist",   //  8
  "cpyf","not", "and",  "or",   "xor",   //  9
  "jmp", "je",  "jne",  "jl",   "jg",    // 10
  "jle", "jge", "call", "ret",  "extl",  // 11
  "extc", "#" };                         // 12

const short opcodeOperandCounts[] =
{   1,     2,     2,      2,      1,     //  1
    1,     1,     1,      1,      1,     //  2
    1,     1,     1,      1,      2,     //  3
    2,     2,     2,      2,      0,     //  4
    0,     0,     0,      0,      2,     //  5
    2,     0,     1,      2,      2,     //  6
    2,     2,     2,      2,      2,     //  7
    2,     2,     2,      2,      2,     //  8
    2,     1,     2,      2,      2,     //  9
    1,     1,     1,      1,      1,     // 10
    1,     1,     1,      1,      1,     // 11
    1,     -1                            // 12
};

enum Id
{
    CLR = 0, // Clears A
    SET,     // Sets a single location to A
    MOVE,    // Copies the value of B to A
    SWAP,    // Swaps the values of A and B
    IN,      // Reads a single character into A
    INS,     // Reads a string into an array of characters pointed to by A
    OUT,     // Outputs the value of A
    OUTS,    // Outputs a string pointed to by A. Output stops at the end of the array or at the first '\0' character
    PUSH,    // Pushes the value of a A to the stack
    POP,     // Pops the value at the top of the stack into A
    INC,     // Increments the value of A
    DEC,     // Decrements the value of A
    NEG,     // Replaces the value of A with -A
    ABS,     // Replaces the value of A with |A|
    ADD,     // Adds B to A
    SUB,     // Takes B away from A
    MUL,     // Multiplies A by B
    DIV,     // Divides A by B
    MOD,     // Replaces A by the remainder of A mod B
    SADD,    // Adds the value at the top of the stack to the value below
    SSUB,    // Takes away the value at the top of the stack from the value below
    SMUL,    // Multiplies the value below the top of the stack by the value at the top
    SDIV,    // Divides the value below the top of the stack by the value at the top
    SMOD,    // Replaces the value below the top of the stack by below mod top
    ALLC,   // Allocates array type A, length B in managed heap. Pointer to array is put into managed out register
    PLA,     // Allows the array pointed to by A to be populated with values, starting with index B
    FNA,     // Finishes array population started by PLA
    ATOA,    // Adds B to the array that is currently being populated
    AEL,     // Puts element at index B of array pointed to by A into the managed out register
    ALEN,    // Puts the length of the array pointed to by B into A
    CPYA,    // Copies array pointed to by B into array pointed to by A. Array length A must be >= array length B
    CNVI,    // Converts B into an integer and puts the value in A
    CNVR,    // Converts B into a real and puts the value in A
    CNVC,    // Converts B into a char and puts the value in A
    CNVB,    // Converts B into a boolean and puts the value in A
    CNVT,    // Converts A into the type of B, replacing A with the new value of A
    DREF,    // Dereferences pointer B and puts the value into A
    CMP,     // Compares A with B
    CMPT,    // Compares just the types of A and B
    IST,     // Compares the type of A with type B
    CPYF,    // Copies the comparison flag B into A
    NOT,     // Replaces A with the logical NOT of A
    AND,     // Replaces A with the logical AND of A and B
    OR,      // Replaces A with the logical OR of A and B
    XOR,     // Replaces A with the logical XOR or A and B
    JMP,     // Jumps to label A
    JE,      // Jumps to label A if in the last comparison, A == B
    JNE,     // Jumps to label A if in the last comparision, A != B
    JL,      // Jumps to label A if in the last comparison, A < B
    JG,      // Jumps to label A if in the last comparison, A > B
    JLE,     // Jumps to label A if in the last comparison, A <= B
    JGE,     // Jumps to label A if in the last comparision, A >= B
    CALL,    // A function call to label A. Pushes the frame stack pointer and jumps to A
    RET,     // Returns from the function call, returning A by pushing it to the top of the stack
    EXTL,    // Loads an extension library, whose name is given by label A
    EXTC     // Calls an extension library function of name A
};

int getOpcodeId(const std::string & opcode);

}

#endif // OPCODES_HPP
