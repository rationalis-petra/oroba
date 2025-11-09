#include "oroba/data/bytecode.hpp"

#include <ostream>

using namespace std;

MessageOp::MessageOp(string _name, uint16_t _num_operands)
    : name(_name)
    , num_operands(_num_operands) { }

OpCode::~OpCode() {
}

OpCode OpCode::push(OrobaObject* literal) {
    OpCode out(literal);
    out.type = OpCodeType::Push;
    return out;
}

OpCode OpCode::impl_message(string messagename, uint16_t num_operands) {
    OpCode out(MessageOp(messagename, num_operands));
    out.type = OpCodeType::ImplMessage;
    return out;
}

OpCode OpCode::expl_message(string messagename, uint16_t num_operands) {
    OpCode out(MessageOp(messagename, num_operands));
    out.type = OpCodeType::ExplMessage;
    return out;
}

OpCode::OpCode(MessageOp op) : payload(op) { }

OpCode::OpCode(OrobaObject* lit) : payload(lit) { } 

void Bytecode::Trace() {
    for (auto op : ops) {
        if (op.type == OpCodeType::Push) {
            OrobaObject* obj = get<OrobaObject*>(op.payload);
            obj->Trace();
        }
    }
}

ostream& operator<<(ostream& out, Bytecode code) {
    out << "bytecode";
    return out;
}
