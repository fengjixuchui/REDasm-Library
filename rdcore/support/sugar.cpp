#include "sugar.h"
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <cctype>

int Sugar::branchDirection(const RDInstruction* instruction, address_t address)
{
    return static_cast<int>(static_cast<std::make_signed<decltype(address)>::type>(address) -
                            static_cast<std::make_signed<decltype(instruction->address)>::type>(instruction->address));
}

address_t Sugar::endAddress(const RDInstruction* instruction) { return instruction->address + instruction->size; }
bool Sugar::isBranch(const RDInstruction* instruction) { return (instruction->type == InstructionType_Call) || (instruction->type == InstructionType_Jump); }

bool Sugar::isUnconditionalJump(const RDInstruction* instruction)
{
    if(instruction->type != InstructionType_Jump) return false;
    return !(instruction->flags & InstructionFlags_Conditional);
}

bool Sugar::isConditionalJump(const RDInstruction* instruction)
{
    if(instruction->type != InstructionType_Jump) return false;
    return instruction->flags & InstructionFlags_Conditional;
}

bool Sugar::isNumeric(const RDOperand* operand)
{
    switch(operand->type)
     {
        case OperandType_Constant:
        case OperandType_Immediate:
        case OperandType_Memory: return true;
        default: break;
    }

    return false;
}

bool Sugar::isCharacter(address_t value) { return (value <= 0xFF) && ::isprint(static_cast<u8>(value)); }

bool Sugar::displacementCanBeAddress(const RDOperand* operand)
{
    return (operand->type == OperandType_Displacement) && (operand->displacement > 0) && !Sugar::isBaseValid(operand) && !Sugar::isIndexValid(operand);
}

bool Sugar::isBaseValid(const RDOperand* operand)
{
    return (operand->type == OperandType_Displacement) && (operand->base != RD_NREG);
}

bool Sugar::isIndexValid(const RDOperand* operand)
{
    return (operand->type == OperandType_Displacement) && (operand->index != RD_NREG);
}
