/*
 * Lexer.hpp
 *
 *  Created on: 7 Feb 2012
 *      Author: Max Foster
 */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>

class Instruction;
class Token;

/* BNF for plain text instructions
 *
 * <instruction> ::= { <label> } { <instruction-part> }
 *
 * <label> ::= <letter> { <character>* }
 *
 * <instruction-part> ::= <opcode> { <operands> }
 *
 * <opcode> ::= <letter> { <letter>* }
 *
 * <operands> ::= <operand> { <operand> }
 *
 * <operand> ::= <constant> | <location> | <pointer>
 *
 * <constant> ::= "#" ( <integer> | <real> | <char> | <boolean> )
 * <integer>  ::= <digit>*
 * <real>     ::= <digit>* "." <digit>*
 * <char>     ::= "'" <character> "'"
 * <boolean>  ::= "T" | "F"
 *
 * <location ::= <stack-location> | <heap-location> | <register>
 *
 * <stack-location> ::= "S" ( "T" | "B" ) { <digit>* }
 *
 * <heap-location> ::= <digit>*
 *
 * <register> ::= "R" ( "P" | "M" )
 *
 * <pointer> ::= "@" <location>
 *
 * No need to explain <letter>, <digit> or <character>
 */

class Lexer
{
public:
    static const std::string opcodeStrings[];

    static int getOpcodeId(const std::string & opcode);
    static const Instruction & tokenize(const std::string & instruction);

    static Token getOpcodeTokenFrom(const std::string & str);
    static Token getOperandTokenFrom(const std::string & str);

private:
    Lexer() {} // To stop instantiation of this class
};

#endif // LEXER_HPP
