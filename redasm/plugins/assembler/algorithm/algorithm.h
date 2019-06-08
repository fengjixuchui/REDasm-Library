#pragma once

#include "../../../types/plugin_types.h"
#include "../../../pimpl.h"
#include "state.h"

#define EXECUTE_STATE(id, value, index, instruction)  this->executeState({ #id, id, static_cast<u64>(value), index, instruction })
#define ENQUEUE_STATE(id, value, index, instruction)  this->enqueueState({ #id, id, static_cast<u64>(value), index, instruction })
#define FORWARD_STATE_VALUE(newid, value, state)      EXECUTE_STATE(newid, value, state->index, state->instruction)
#define FORWARD_STATE(newid, state)                   FORWARD_STATE_VALUE(newid, state->u_value, state)
#define DECODE_STATE(address)                         ENQUEUE_STATE(Algorithm::DecodeState, address, REDasm::npos, nullptr)

namespace REDasm {

class AlgorithmImpl;
class Disassembler;

class Algorithm: public Object
{
    PIMPL_DECLARE_PRIVATE(Algorithm)

    DEFINE_STATES(DecodeState,
                  JumpState, CallState, BranchState, BranchMemoryState,
                  AddressTableState, MemoryState, PointerState, ImmediateState,
                  UserState = USER_STATE_START)

    public:
        enum: size_t { OK, SKIP, FAIL };

    protected:
        Algorithm(AlgorithmImpl* p);

    public:
        Algorithm(Disassembler* disassembler);
        virtual ~Algorithm() = default;
        Disassembler* disassembler() const;
        size_t disassembleInstruction(address_t address, const InstructionPtr& instruction);
        void done(address_t address);
        void enqueue(address_t address);
        void analyze();
        bool hasNext() const;
        void next();

    protected:
        void enqueueState(const State& state);
        void executeState(const State& state);
        virtual bool validateState(const State& state) const;
        virtual void onNewState(const State *state) const;
        virtual void validateTarget(const InstructionPtr& instruction) const;
        virtual void onDecoded(const InstructionPtr& instruction);
        virtual void onDecodeFailed(const InstructionPtr& instruction);
        virtual void onDecodedOperand(const Operand* op, const InstructionPtr& instruction);
        virtual void onEmulatedOperand(const Operand* op, const InstructionPtr& instruction, u64 value);

    protected:
        virtual void decodeState(const State *state);
        virtual void jumpState(const State *state);
        virtual void callState(const State *state);
        virtual void branchState(const State *state);
        virtual void branchMemoryState(const State *state);
        virtual void addressTableState(const State *state);
        virtual void memoryState(const State* state);
        virtual void pointerState(const State* state);
        virtual void immediateState(const State* state);
};

} // namespace REDasm
