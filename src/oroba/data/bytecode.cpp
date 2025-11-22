#include "oroba/data/bytecode.hpp"

#include <ostream>

#include "oroba/objects/object.hpp"

using namespace std;

MessageOp::MessageOp(string _name, uint16_t _num_operands)
    : name(_name)
    , num_operands(_num_operands) { }


MakeBlock::MakeBlock(unordered_map<string, SlotDescriptor> _slots,
                     vector<string> _to_initialize,
                     std::vector<string> _args,
                     shared_ptr<Bytecode> _body)
    : slots(_slots)
    , to_initialize(_to_initialize)
    , args(_args)
    , body(_body) { }

MakeObject::MakeObject(unordered_map<string, SlotDescriptor> _slots,
                       unordered_map<string, MethodDescriptor> _methods,
                       vector<string> _to_initialize)
    : slots(_slots)
    , methods(_methods)
    , to_initialize(_to_initialize) { }

OpCode::~OpCode() {
}

OpCode OpCode::push(OrobaObject* literal) {
    OpCode out(literal);
    out.type = OpCodeType::Push;
    return out;
}

OpCode OpCode::pop() {
    OpCode out(nullptr);
    out.type = OpCodeType::Pop;
    return out;
}

OpCode OpCode::swap() {
    OpCode out(nullptr);
    out.type = OpCodeType::Swap;
    return out;
}

OpCode OpCode::make_object(unordered_map<string, SlotDescriptor> slots,
                           unordered_map<string, MethodDescriptor> methods,
                           vector<string> to_initialize) {
    OpCode out(MakeObject(slots, methods, to_initialize));
    out.type = OpCodeType::MakeObject;
    return out;
}

OpCode OpCode::make_block(unordered_map<string, SlotDescriptor> slots,
                          vector<string> to_initialize,
                          vector<string> args,
                          shared_ptr<Bytecode> body) {
    OpCode out(MakeBlock(slots, to_initialize, args, body));
    out.type = OpCodeType::MakeBlock;
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
OpCode::OpCode(MakeObject obj_desc) : payload(obj_desc) { }
OpCode::OpCode(MakeBlock block_desc) : payload(block_desc) { }

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
