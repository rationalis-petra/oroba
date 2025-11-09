#ifndef __OROBA_DATA_BYTECODE_HPP
#define __OROBA_DATA_BYTECODE_HPP

// Expression evaluation.
// use stack-based opcode
//   
//   impl-message <n-operands> <name>
//   expl-message <n-operands> <name>
//   push <literal>
//   pop
//   assign-r  <name>     
//   assign-rw <name>     
//   start-object
//   end-object

#include <vector>
#include <cstdint>

#include "oroba/object/object.hpp"

enum class OpCodeType {
    ImplMessage,
    ExplMessage,
    Push, 
    Pop,
    AssignR,
    AssignRW,
    StartObj,
    EndObj,
    Nop, 
};

struct MessageOp {
    uint16_t num_operands;
    string name;
};

struct OpCode {
    OpCode();
    OpCode(const OpCode& rhs);
    ~OpCode();

    static OpCode push(OrobaObject* literal);

    OpCodeType type;
    union {
        MessageOp message;
        OrobaObject* literal;
    };
};

struct Bytecode {
    std::vector<OpCode> ops;
};

std::ostream& operator<<(std::ostream& out, Bytecode code);

#endif
