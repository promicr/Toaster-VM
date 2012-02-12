/*
 * Lexer.hpp
 *
 *  Created on: 7 Feb 2012
 *      Author: Max Foster
 */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>

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
 * <operand> ::= <constant> | <location> | <pointer> | <data-type> | <comparison-flag-id>  | <label> | "nil"
 *
 * <constant> ::= "#" ( <integer> | <real> | <char> | <boolean> )
 * <integer>  ::= <digit>*
 * <real>     ::= <digit>* "." <digit>*
 * <char>     ::= "'" <character> "'"
 * <boolean>  ::= "T" | "F"
 *
 * <location ::= <stack-location> | <heap-location> | <register>
 *
 * <stack-location> ::= "S" ( "T" | "B" | "N" ) { <digit>* }
 *
 * <heap-location> ::= <digit>*
 *
 * <register> ::= "R" ( "P" | "M" )
 *
 * <pointer> ::= "@" <location>
 *
 * <data-type> ::= "$" ( "i" | "r" | "c" | "b" | "p" )
 *
 * <comparison-flag-id> ::= "?" ( "eq" | "ne" | "lt" | "gt" | "le" | "ge" )
 *
 * No need to explain <letter>, <digit> or <character>
 */

class Instruction;
class Token;
class Machine;

namespace Lexer
{

const Instruction & tokenize(const std::string & instruction, Machine & machine);

Token getOpcodeToken(const std::string & str);
Token getOperandToken(const std::string & str, Machine & machine);

}

#endif // LEXER_HPP
