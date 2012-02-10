/*
 * Interpreter.cpp
 *
 *  Created on: 5 Feb 2012
 *      Author: Max Foster
 */

#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>

#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Instruction.hpp"
#include "Machine.hpp"

Interpreter::Interpreter(Machine & machine)
    : machine(machine)
{
    unsigned line = 0;
    std::string buffer;
    while (true)
    {
        ++line;
        std::cout << line << " >> ";
        getline(std::cin, buffer);

        size_t commentPos = buffer.find_first_of(';');
        if (commentPos != std::string::npos)
            buffer = buffer.substr(0, commentPos);

        if (buffer == "END") break;
        fileContents << buffer << '\n';
    }
    instructions.reserve(line);
}

Interpreter::Interpreter(Machine & machine, const char * fileName)
    : machine(machine)
{
    if ((fileName == NULL) || (strlen(fileName) == 0))
        throw(std::runtime_error("Parser::Parser: Invalid file name given"));

    std::fstream file(fileName, std::ios_base::in);
    if (file.is_open())
    {
        unsigned line = 0;
        std::string buffer;
        do
        {
            ++line;
            getline(file, buffer);

            size_t commentPos = buffer.find_first_of(';');
            if (commentPos != std::string::npos)
                buffer = buffer.substr(0, commentPos);

            fileContents << buffer << '\n';
        } while (!file.eof());
        instructions.reserve(line);

        file.close();
    }
    else throw(std::runtime_error("Parser::Parser: File could not be opened"));
}

void Interpreter::run()
{
    unsigned line = 0;
    std::string instruction;
    while (true)
    {
        getline(fileContents, instruction);
        if (fileContents.eof()) break;

        const Instruction * i;
        try { i = &Lexer::tokenize(instruction); } // Instruction returned is static, so using pointer is fine
        catch (const std::exception & e)
        {
            std::cout << "Error on line " << line + 1 << std::endl
                      << instruction << std::endl
                      << e.what() << std::endl
                      << "Execution halted" << std::endl;
            return;
        }

        if (!i->label.isNull()) machine.addLabel(i->label.labelData(), line);
        instructions.push_back(*i); // Even though the Instruction might contain nothing, we still need
                                    // to add it in order to give helpful error messages (i.e to show line number)
        ++line;
    }

    try { machine.jump("main"); }
    catch (const std::exception & e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "Label 'main' not found" << std::endl
                  << "Execution halted" << std::endl;
        return;
    }

    unsigned programCounter = machine.programCounter();
    while (programCounter < instructions.size())
    {
        try { execute(instructions[programCounter]); }
        catch (const std::exception & e)
        {
            std::cout << "Error on line " << programCounter + 1 << std::endl
                      << e.what() << std::endl
                      << "Execution halted" << std::endl;
            return;
        }

        if (programCounter == machine.programCounter()) ++machine.programCounter(); // i.e. if there were no jumps
        programCounter = machine.programCounter();
    }

    getline(std::cin, instruction);
}

std::string typeString(const Token::Type type)
{
    switch (type)
    {
    case Token::T_OPCODE:                       return "opcode";
    case Token::T_OPERAND_CONST_INT:            return "const-int";
    case Token::T_OPERAND_CONST_REAL:           return "const-real";
    case Token::T_OPERAND_CONST_CHAR:           return "const-char";
    case Token::T_OPERAND_CONST_BOOL:           return "const-bool";
    case Token::T_OPERAND_DATA_TYPE:            return "data-type";
    case Token::T_OPERAND_STACK_TOP:            return "stack-top";
    case Token::T_OPERAND_STACK_BOTTOM:         return "stack-bottom";
    case Token::T_OPERAND_HEAP_LOCATION:        return "heap-location";
    case Token::T_OPERAND_PRIMARY_REGISTER:     return "primary-register";
    case Token::T_OPERAND_MANAGED_OUT_REGISTER: return "managed-register";
    case Token::T_OPERAND_NIL:                  return "operand-nil";
    case Token::T_LABEL:                        return "label";
    case Token::T_NULL:                         return "null";
    default: return "ERROR";
    }
}

std::string valueString(const Token & token)
{
    std::stringstream stream;
    switch (token.type())
    {
    case Token::T_OPCODE:                       stream << (int)token.opcodeData(); break;
    case Token::T_OPERAND_CONST_INT:            stream << token.integerData(); break;
    case Token::T_OPERAND_CONST_REAL:           stream << token.realData(); break;
    case Token::T_OPERAND_CONST_CHAR:           stream << token.charData(); break;
    case Token::T_OPERAND_CONST_BOOL:           stream << (token.booleanData() ? "true" : "false"); break;
    case Token::T_OPERAND_DATA_TYPE:            stream << (token.dataTypeData()); break;
    case Token::T_OPERAND_STACK_TOP:            stream << token.stackPositionData(); break;
    case Token::T_OPERAND_STACK_BOTTOM:         stream << token.stackPositionData(); break;
    case Token::T_OPERAND_HEAP_LOCATION:        stream << token.heapLocationData(); break;
    case Token::T_OPERAND_PRIMARY_REGISTER:     stream << "primary-register"; break;
    case Token::T_OPERAND_MANAGED_OUT_REGISTER: stream << "managed-register"; break;
    case Token::T_OPERAND_NIL:                  stream << "operand-nil"; break;
    case Token::T_LABEL:                        stream << token.labelData(); break;
    case Token::T_NULL:                         stream << "null"; break;
    default: stream << "ERROR";
    }
    return stream.str();
}

void Interpreter::outputTokenData(const std::string & instruction)
{
    const Instruction & i = Lexer::tokenize(instruction);
    std::cout << (i.label.isNull() ? "" : typeString(i.label.type()) + " ")
              << typeString(i.opcode.type()) << " "
              << (i.operand1.isPointer() ? "@" : "") + typeString(i.operand1.type()) << " "
              << (i.operand2.isPointer() ? "@" : "") + typeString(i.operand2.type()) << std::endl
              << (i.label.isNull() ? "" : valueString(i.label) + " ")
              << valueString(i.opcode) << " "
              << valueString(i.operand1) << " "
              << valueString(i.operand2) << std::endl;
}

inline bool firstOperandIsLabel(const Instruction & instruction, const Block * const operand2)
{
    return (instruction.operand1.type() == Token::T_LABEL) && (operand2 == NULL) && (instruction.operand2.isNull());
}

void Interpreter::execute(const Instruction & instruction)
{
    if (instruction.opcode.isNull()) return;

    machine.operand1IsPointer() = instruction.operand1.isPointer();
    machine.operand2IsPointer() = instruction.operand2.isPointer();

    bool operand1IsLabel, operand2IsLabel;
    Block * operand1Block = getBlockFromToken(instruction.operand1, operand1IsLabel, 1),
            * operand2Block = getBlockFromToken(instruction.operand2, operand2IsLabel, 2);

    short requiredOperandNumber = Lexer::opcodeOperandCounts[instruction.opcode.opcodeData()];
    if (operand1Block != NULL) --requiredOperandNumber;
    if (operand2Block != NULL) --requiredOperandNumber;

    bool error = false, instructionFinished = false;
    if (requiredOperandNumber != 0)
    {
        switch (instruction.opcode.opcodeData())
        {
        // pop
        case 9:
            if ((instruction.operand1.type() == Token::T_OPERAND_NIL)
                    && (operand2Block == NULL) && (instruction.operand2.isNull()))
            {
                machine.pop(Machine::L_NIL);
                instructionFinished = true;
            }
            else error = true;
            break;

        // allc (allocate)
        case 24:
            if ((instruction.operand1.type() == Token::T_OPERAND_DATA_TYPE) && (operand2Block != NULL))
            {
                machine.allocate(instruction.operand1.dataTypeData(), *operand2Block);
                instructionFinished = true;
            }
            else error = true;
            break;

        // ist (is type?)
        case 39:
            if ((operand1Block != NULL) && (instruction.operand2.type() == Token::T_OPERAND_DATA_TYPE))
            {
                machine.isDataType(*operand1Block, instruction.operand2.dataTypeData());
                instructionFinished = true;
            }
            else error = true;
            break;

        // cpyf (copy comparison flag)
        case 40:
            if ((operand1Block != NULL) && (instruction.operand2.type() == Token::T_OPERAND_COMPARISON_FLAG_ID))
            {
                machine.copyFlag(*operand1Block, instruction.operand2.comparisonFlagData());
                instructionFinished = true;
            }
            else error = true;
            break;

        // jump
        case 45:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.jump(instruction.operand1.labelData());
                instructionFinished = true;
            }
            else error = true;
            break;

        // je
        case 46:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_EQUAL);
                instructionFinished = true;
            }
            else error = true;
            break;

        // jne
        case 47:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_NOT_EQUAL);
                instructionFinished = true;
            }
            else error = true;
            break;

        // jl
        case 48:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_LESS);
                instructionFinished = true;
            }
            else error = true;
            break;

        // jg
        case 49:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_GREATER);
                instructionFinished = true;
            }
            else error = true;
            break;

        // jle
        case 50:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_LESS_EQUAL);
                instructionFinished = true;
            }
            else error = true;
            break;

        // jge
        case 51:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_GREATER_EQUAL);
                instructionFinished = true;
            }
            else error = true;
            break;

        // call (function call)
        case 52:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.call(instruction.operand1.labelData());
                instructionFinished = true;
            }
            else error = true;
            break;

        // extl (load extension)
        case 54:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.loadExtension(instruction.operand1.labelData());
                instructionFinished = true;
            }
            else error = true;
            break;

        // extc (extension call)
        case 55:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.extensionCall(instruction.operand1.labelData());
                instructionFinished = true;
            }
            else error = true;
            break;

        default: error = true;
        }
    }
    if (error)
    {
        if (requiredOperandNumber < 0) throw(std::runtime_error("Interpreter::execute: Too many operands given"));
        else if (requiredOperandNumber > 0) throw(std::runtime_error("Interpreter:execute: Too few operands given"));
        else if ((operand1Block == NULL) && (operand2Block != NULL))
            throw(std::runtime_error("First operand given is invalid"));
    }

    if (instructionFinished) return;

    switch (instruction.opcode.opcodeData())
    {
    // clr
    case 0: machine.clear(*operand1Block); break;
    // set
    case 1: machine.set(*operand1Block, *operand2Block); break;
    // move
    case 2: machine.move(*operand1Block, *operand2Block); break;
    // swap
    case 3: machine.swap(*operand1Block, *operand2Block); break;
    // in (read)
    case 4: machine.read(*operand1Block); break;
    // ins (readString)
    case 5: machine.readString(*operand1Block); break;
    // out (write)
    case 6: machine.write(*operand1Block); break;
    // outs (writeString)
    case 7: machine.writeString(*operand1Block); break;
    // push
    case 8: machine.push(*operand1Block); break;
    // pop
    case 9: machine.pop(*operand1Block); break;
    // inc
    case 10: machine.increment(*operand1Block); break;
    // dec
    case 11: machine.decrement(*operand1Block); break;
    // neg
    case 12: machine.negate(*operand1Block); break;
    // abs
    case 13: machine.absolute(*operand1Block); break;
    // add
    case 14: machine.add(*operand1Block, *operand2Block); break;
    // sub
    case 15: machine.subtract(*operand1Block, *operand2Block); break;
    // mul
    case 16: machine.multiply(*operand1Block, *operand2Block); break;
    // div
    case 17: machine.divide(*operand1Block, *operand2Block); break;
    // mod
    case 18: machine.modulo(*operand1Block, *operand2Block); break;
    // sadd (stack add)
    case 19: machine.stackAdd(); break;
    // ssub (stack sub)
    case 20: machine.stackSubtract(); break;
    // smul (stack mul)
    case 21: machine.stackMultiply(); break;
    // sdiv (stack div)
    case 22: machine.stackDivide(); break;
    // smod (stack mod)
    case 23: machine.stackModulo(); break;
    // allc
    case 24: break; // handled above
    // pla (start populating array)
    case 25: machine.startPopulatingArray(*operand1Block); break;
    // fna (finish populating array)
    case 26: machine.stopPopulatingArray(); break;
    // atoa (add to array)
    case 27: machine.addToArray(*operand1Block); break;
    // ael (array element)
    case 28: machine.getArrayElement(*operand1Block, *operand2Block); break;
    // alen (array length)
    case 29: machine.getArrayLength(*operand1Block, *operand2Block); break;
    // cpya (copy array)
    case 30: machine.copyArray(*operand1Block, *operand2Block); break;
    // cnvi (convert to integer)
    case 31: machine.convert(*operand1Block, *operand2Block, Block::DT_INTEGER); break;
    // cnvr (convert to real)
    case 32: machine.convert(*operand1Block, *operand2Block, Block::DT_REAL); break;
    // cnvc (convert to char)
    case 33: machine.convert(*operand1Block, *operand2Block, Block::DT_CHAR); break;
    // cnvb (convert to boolean)
    case 34: machine.convert(*operand1Block, *operand2Block, Block::DT_BOOLEAN); break;
    // cnvt (convert to type of)
    case 35: machine.convertToDataTypeOf(*operand1Block, *operand2Block); break;
    // dref (dereference pointer)
    case 36: machine.dereference(*operand1Block, *operand2Block); break;
    // cmp (compare)
    case 37: machine.compare(*operand1Block, *operand2Block); break;
    // cmpt (compare type)
    case 38: machine.compareDataType(*operand1Block, *operand2Block); break;
    // ist
    case 39: break; // handled above
    // cpyf
    case 40: break; // handled above
    // not (logical not)
    case 41: machine.logicalNot(*operand1Block); break;
    // and (logical and)
    case 42: machine.logicalAnd(*operand1Block, *operand2Block); break;
    // or (logical or)
    case 43: machine.logicalOr(*operand1Block, *operand2Block); break;
    // xor (logical xor)
    case 44: machine.logicalXor(*operand1Block, *operand2Block); break;
    // jump
    case 45: // all handled above
    // je
    case 46:
    // jne
    case 47:
    // jl
    case 48:
    // jg
    case 49:
    // jle
    case 50:
    // jge
    case 51:
    // call
    case 52: break; // handled above
    // ret
    case 53: machine.returnFromCall(*operand1Block); break;
    // extl
    case 54: // handled above
    // extc (extension call)
    case 55: break;
    }
}

Block * Interpreter::getBlockFromToken(const Token & token, bool & isLabel, short operandNumber)
{
    static Block block[2];

    isLabel = false;
    if (token.isNull() || (operandNumber < 1) || (operandNumber > 2)) return NULL;
    --operandNumber;
    switch (token.type())
    {
    case Token::T_OPERAND_CONST_INT:            block[operandNumber].setToInteger(token.integerData()); break;
    case Token::T_OPERAND_CONST_REAL:           block[operandNumber].setToReal(token.realData()); break;
    case Token::T_OPERAND_CONST_CHAR:           block[operandNumber].setToChar(token.charData()); break;
    case Token::T_OPERAND_CONST_BOOL:           block[operandNumber].setToBoolean(token.booleanData()); break;
    case Token::T_OPERAND_STACK_TOP:            return &machine.stack().fromTop(token.stackPositionData());
    case Token::T_OPERAND_STACK_BOTTOM:         return &machine.stack().at(token.stackPositionData());
    case Token::T_OPERAND_STACK_NEGATIVE:       return &machine.stack().fromTopBelow(token.stackPositionData());
    case Token::T_OPERAND_HEAP_LOCATION:        return &machine.unmanagedHeap().blockAt(token.heapLocationData());
    case Token::T_OPERAND_PRIMARY_REGISTER:     return &machine.primaryRegister(); break;
    case Token::T_OPERAND_MANAGED_OUT_REGISTER: return &machine.managedOutRegister(); break;
    case Token::T_LABEL:                        isLabel = true; return NULL;
    default: return NULL;
    }

    return &block[operandNumber];
}
