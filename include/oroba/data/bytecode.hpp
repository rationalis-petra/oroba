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
#include <unordered_map>
#include <vector>
#include <memory>
#include <variant>

#include "oroba/object/object.hpp"

enum class OpCodeType {
    ImplMessage,
    ExplMessage,
    Push, 
    Pop,
    MakeBlock,
    MakeObject,
    MakeMethod,
};

struct Bytecode;

struct MessageOp {
    MessageOp(std::string _name, uint16_t _num_operands);

    std::string name;
    uint16_t num_operands;
};

struct MakeBlock {
public:
    MakeBlock(std::unordered_map<std::string, SlotDescriptor> slots,
              std::vector<std::string> to_initialize,
              std::shared_ptr<Bytecode> code);

    std::unordered_map<std::string, SlotDescriptor> slots;
    std::vector<std::string> to_initialize;
    std::shared_ptr<Bytecode> code;
};

struct MakeMethod {
    MakeMethod(std::unordered_map<std::string, SlotDescriptor> slots,
               std::vector<std::string> to_initialize,
               std::shared_ptr<Bytecode> code);

    std::unordered_map<std::string, SlotDescriptor> slots;
    std::vector<std::string> to_initialize;
    std::shared_ptr<Bytecode> code;
};

struct MakeObject {
    MakeObject(std::unordered_map<std::string, SlotDescriptor> slots,
               std::vector<std::string> to_initialize);

    std::unordered_map<std::string, SlotDescriptor> slots;
    std::vector<std::string> to_initialize;
};

struct OpCode {
    ~OpCode();

    static OpCode push(OrobaObject* literal);
    static OpCode make_object(std::unordered_map<std::string, SlotDescriptor> slots,
                              std::vector<std::string> to_initialize);
    static OpCode make_method(std::unordered_map<std::string, SlotDescriptor> slots,
                              std::vector<std::string> to_initialize,
                              std::shared_ptr<Bytecode> code);
    static OpCode make_block(std::unordered_map<std::string, SlotDescriptor> slots,
                              std::vector<std::string> to_initialize,
                              std::shared_ptr<Bytecode> code);
    static OpCode impl_message(std::string messagename, uint16_t num_operands);
    static OpCode expl_message(std::string messagename, uint16_t num_operands);

    OpCodeType type;
    std::variant<MessageOp, OrobaObject*, MakeObject, MakeMethod, MakeBlock> payload;

private: 
    OpCode(MessageOp op);
    OpCode(OrobaObject* lit);
    OpCode(MakeObject obj_desc);
    OpCode(MakeMethod method_desc);
    OpCode(MakeBlock block_desc);
};

struct Bytecode : public Tracer {
    std::vector<OpCode> ops;

    virtual void Trace() override;
};

std::ostream& operator<<(std::ostream& out, Bytecode code);

#endif
