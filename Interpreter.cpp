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
#include "Opcodes.hpp"

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

    short requiredOperandNumber = Opcodes::opcodeOperandCounts[instruction.opcode.opcodeData()];
    if (operand1Block != NULL) --requiredOperandNumber;
    if (operand2Block != NULL) --requiredOperandNumber;

    bool error = false, instructionFinished = false;
    if (requiredOperandNumber != 0)
    {
        switch (instruction.opcode.opcodeData())
        {
        case Opcodes::POP:
            if ((instruction.operand1.type() == Token::T_OPERAND_NIL)
                    && (operand2Block == NULL) && (instruction.operand2.isNull()))
            {
                machine.pop(Machine::L_NIL);
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::ALLC:
            if ((instruction.operand1.type() == Token::T_OPERAND_DATA_TYPE) && (operand2Block != NULL))
            {
                machine.allocate(instruction.operand1.dataTypeData(), *operand2Block);
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::IST:
            if ((operand1Block != NULL) && (instruction.operand2.type() == Token::T_OPERAND_DATA_TYPE))
            {
                machine.isDataType(*operand1Block, instruction.operand2.dataTypeData());
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::CPYF:
            if ((operand1Block != NULL) && (instruction.operand2.type() == Token::T_OPERAND_COMPARISON_FLAG_ID))
            {
                machine.copyFlag(*operand1Block, instruction.operand2.comparisonFlagData());
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::JMP:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.jump(instruction.operand1.labelData());
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::JE:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_EQUAL);
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::JNE:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_NOT_EQUAL);
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::JL:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_LESS);
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::JG:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_GREATER);
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::JLE:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_LESS_EQUAL);
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::JGE:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.conditionalJump(instruction.operand1.labelData(), CFR::F_GREATER_EQUAL);
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::CALL:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.call(instruction.operand1.labelData());
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::EXTL:
            if (firstOperandIsLabel(instruction, operand2Block))
            {
                machine.loadExtension(instruction.operand1.labelData());
                instructionFinished = true;
            }
            else error = true;
            break;

        case Opcodes::EXTC:
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
    case Opcodes::CLR:  machine.clear(*operand1Block); break;
    case Opcodes::SET:  machine.set(*operand1Block, *operand2Block); break;
    case Opcodes::MOVE: machine.move(*operand1Block, *operand2Block); break;
    case Opcodes::SWAP: machine.swap(*operand1Block, *operand2Block); break;
    case Opcodes::IN:   machine.read(*operand1Block); break;
    case Opcodes::INS:  machine.readString(*operand1Block); break;
    case Opcodes::OUT:  machine.write(*operand1Block); break;
    case Opcodes::OUTS: machine.writeString(*operand1Block); break;
    case Opcodes::PUSH: machine.push(*operand1Block); break;
    case Opcodes::POP:  machine.pop(*operand1Block); break;
    case Opcodes::INC:  machine.increment(*operand1Block); break;
    case Opcodes::DEC:  machine.decrement(*operand1Block); break;
    case Opcodes::NEG:  machine.negate(*operand1Block); break;
    case Opcodes::ABS:  machine.absolute(*operand1Block); break;
    case Opcodes::ADD:  machine.add(*operand1Block, *operand2Block); break;
    case Opcodes::SUB:  machine.subtract(*operand1Block, *operand2Block); break;
    case Opcodes::MUL:  machine.multiply(*operand1Block, *operand2Block); break;
    case Opcodes::DIV:  machine.divide(*operand1Block, *operand2Block); break;
    case Opcodes::MOD:  machine.modulo(*operand1Block, *operand2Block); break;
    case Opcodes::SADD: machine.stackAdd(); break;
    case Opcodes::SSUB: machine.stackSubtract(); break;
    case Opcodes::SMUL: machine.stackMultiply(); break;
    case Opcodes::SDIV: machine.stackDivide(); break;
    case Opcodes::SMOD: machine.stackModulo(); break;
    case Opcodes::ALLC: break; // handled above
    case Opcodes::PLA:  machine.startPopulatingArray(*operand1Block, *operand2Block); break;
    case Opcodes::FNA:  machine.stopPopulatingArray(); break;
    case Opcodes::ATOA: machine.addToArray(*operand1Block); break;
    case Opcodes::AEL:  machine.getArrayElement(*operand1Block, *operand2Block); break;
    case Opcodes::ALEN: machine.getArrayLength(*operand1Block, *operand2Block); break;
    case Opcodes::CPYA: machine.copyArray(*operand1Block, *operand2Block); break;
    case Opcodes::CNVI: machine.convert(*operand1Block, *operand2Block, Block::DT_INTEGER); break;
    case Opcodes::CNVR: machine.convert(*operand1Block, *operand2Block, Block::DT_REAL); break;
    case Opcodes::CNVC: machine.convert(*operand1Block, *operand2Block, Block::DT_CHAR); break;
    case Opcodes::CNVB: machine.convert(*operand1Block, *operand2Block, Block::DT_BOOLEAN); break;
    case Opcodes::CNVT: machine.convertToDataTypeOf(*operand1Block, *operand2Block); break;
    case Opcodes::DREF: machine.dereference(*operand1Block, *operand2Block); break;
    case Opcodes::CMP:  machine.compare(*operand1Block, *operand2Block); break;
    case Opcodes::CMPT: machine.compareDataType(*operand1Block, *operand2Block); break;
    case Opcodes::IST:  break; // handled above
    case Opcodes::CPYF: break; // handled above
    case Opcodes::NOT:  machine.logicalNot(*operand1Block); break;
    case Opcodes::AND:  machine.logicalAnd(*operand1Block, *operand2Block); break;
    case Opcodes::OR:   machine.logicalOr(*operand1Block, *operand2Block); break;
    case Opcodes::XOR:  machine.logicalXor(*operand1Block, *operand2Block); break;
    case Opcodes::JMP:  // all handled above
    case Opcodes::JE:
    case Opcodes::JNE:
    case Opcodes::JL:
    case Opcodes::JG:
    case Opcodes::JLE:
    case Opcodes::JGE:
    case Opcodes::CALL: break; // handled above
    case Opcodes::RET:  machine.returnFromCall(*operand1Block); break;
    case Opcodes::EXTL: // handled above
    case Opcodes::EXTC: break;
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
