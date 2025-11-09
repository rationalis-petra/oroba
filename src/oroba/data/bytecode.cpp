#include "oroba/data/bytecode.hpp"

#include <ostream>

OpCode::OpCode() : type(OpCodeType::Nop) { }

OpCode::OpCode(const OpCode& rhs) {
    type = rhs.type;
    switch (type) {
    case OpCodeType::ImplMessage:
    case OpCodeType::ExplMessage:
        message = rhs.message;
        break;
    case OpCodeType::Push:
        literal = rhs.literal;
        break;
    default:
        break;
    }
}

OpCode::~OpCode() {
    switch (type) {
    case OpCodeType::ImplMessage:
    case OpCodeType::ExplMessage:
        message.~MessageOp();
        break;
    default:
        break;
    }
}

OpCode OpCode::push(OrobaObject* literal) {
    OpCode out;
    out.type = OpCodeType::Push;
    out.literal = literal;
    return out;
}

std::ostream& operator<<(std::ostream& out, Bytecode code) {
    out << "bytecode";
    return out;
}
