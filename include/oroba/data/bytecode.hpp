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

#include <cstdint>
#include <vector>
#include <variant>

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
};

struct MessageOp {
    MessageOp(std::string _name, uint16_t _num_operands);

    std::string name;
    uint16_t num_operands;
};

struct OpCode {
    ~OpCode();

    static OpCode push(OrobaObject* literal);
    static OpCode impl_message(std::string messagename, uint16_t num_operands);
    static OpCode expl_message(std::string messagename, uint16_t num_operands);

    OpCodeType type;
    std::variant<MessageOp, OrobaObject*> payload;

private: 
    OpCode(MessageOp op);
    OpCode(OrobaObject* lit);
};

struct Bytecode : public Tracer {
    std::vector<OpCode> ops;

    virtual void Trace() override;
};

std::ostream& operator<<(std::ostream& out, Bytecode code);

#endif
