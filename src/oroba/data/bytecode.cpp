#include "oroba/data/bytecode.hpp"

#include <ostream>

using namespace std;

MessageOp::MessageOp(string _name, uint16_t _num_operands)
    : name(_name)
    , num_operands(_num_operands) { }


MakeBlock::MakeBlock(std::unordered_map<std::string, SlotDescriptor> _slots,
          std::vector<std::string> _to_initialize,
          std::shared_ptr<Bytecode> _code)
    : slots(_slots)
    , to_initialize(_to_initialize)
    , code(_code) { }

MakeMethod::MakeMethod(std::unordered_map<std::string, SlotDescriptor> _slots,
          std::vector<std::string> _to_initialize,
          std::shared_ptr<Bytecode> _code)
    : slots(_slots)
    , to_initialize(_to_initialize)
    , code(_code) { }

MakeObject::MakeObject(std::unordered_map<std::string, SlotDescriptor> _slots,
                       std::vector<std::string> _to_initialize)
    : slots(_slots)
    , to_initialize(_to_initialize) { }

OpCode::~OpCode() {
}

OpCode OpCode::push(OrobaObject* literal) {
    OpCode out(literal);
    out.type = OpCodeType::Push;
    return out;
}

OpCode OpCode::make_object(std::unordered_map<std::string, SlotDescriptor> slots,
                          std::vector<std::string> to_initialize) {
    OpCode out(MakeObject(slots, to_initialize));
    out.type = OpCodeType::MakeObject;
    return out;
}

OpCode OpCode::make_method(std::unordered_map<std::string, SlotDescriptor> slots,
                          std::vector<std::string> to_initialize,
                          std::shared_ptr<Bytecode> code) {
    OpCode out(MakeMethod(slots, to_initialize, code));
    out.type = OpCodeType::MakeMethod;
    return out;
}

OpCode OpCode::make_block(std::unordered_map<std::string, SlotDescriptor> slots,
                         std::vector<std::string> to_initialize,
                         std::shared_ptr<Bytecode> code) {
    OpCode out(MakeBlock(slots, to_initialize, code));
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
OpCode::OpCode(MakeMethod method_desc) : payload(method_desc) { }
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
