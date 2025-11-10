#include "oroba/eval.hpp"

#include <vector>

#include "oroba/data/error.hpp"

using namespace std;

OrobaObject* eval(Bytecode& code, OrobaObject* init, LocalCollector& collector) {
    vector<OrobaObject*> activation_objects;
    activation_objects.push_back(init);

    vector<OrobaObject*> stack;

    for (auto& op : code.ops) {
        if (collector.ShouldCollect()) {
            for (auto obj : stack) collector.AddTempRoot(obj);
            for (auto obj : activation_objects) collector.AddTempRoot(obj);
            collector.AddTempRoot(&code);
            collector.TraceRoots();
            collector.Collect();
        }

        switch (op.type) {
        case OpCodeType::ImplMessage: {
            MessageOp message = get<MessageOp>(op.payload);
            vector args(stack.end() - message.num_operands, stack.end());
            stack.resize(stack.size() - message.num_operands);
            OrobaObject* result = activation_objects.back()->SendMessage(true, message.name, args, collector);
            stack.push_back(result);
            break;
        }
        case OpCodeType::ExplMessage: {
            MessageOp message = get<MessageOp>(op.payload);
            vector args(stack.end() - message.num_operands, stack.end());
            stack.resize(stack.size() - message.num_operands);
            OrobaObject* target = stack.back();
            stack.pop_back();
            OrobaObject* result = target->SendMessage(false, message.name, args, collector);
            stack.push_back(result);
            break;
        }
        case OpCodeType::Push: {
            OrobaObject* literal = get<OrobaObject*>(op.payload);
            stack.push_back(literal);
            break;
        }
        case OpCodeType::Pop: {
            stack.pop_back();
            break;
        }
        case OpCodeType::MakeBlock: {
            throw InternalError("not implemented: eval make block");
            break;
        }
        case OpCodeType::MakeMethod: {
            throw InternalError("not implemented: make method");
            break;
        }
        case OpCodeType::MakeObject: {
            MakeObject mo = get<MakeObject>(op.payload);
            OrobaObject* obj = new OrobaObject;
            collector.Add(obj);
            stack.push_back(obj);
            break;
        }
        default:
            throw InternalError("unrecognized bytecode");
            break;
        }
    }

    if (!stack.empty()) {
        return stack.back();
    } else {
        throw InternalError("Value stack not empty when returning!");
    }
}
