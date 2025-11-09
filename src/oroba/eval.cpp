#include "oroba/eval.hpp"

#include <vector>

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
            OrobaObject* result = activation_objects.back()->SendInternalMessage(message.name, args, collector);
            stack.push_back(result);
            break;
        }
        case OpCodeType::ExplMessage: {
            MessageOp message = get<MessageOp>(op.payload);
            vector args(stack.end() - message.num_operands, stack.end());
            stack.resize(stack.size() - message.num_operands);
            OrobaObject* target = stack.back();
            stack.pop_back();
            OrobaObject* result = target->SendExternalMessage(message.name, args, collector);
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
        case OpCodeType::AssignR: {
            throw "not implemented: eval AssignR";
            break;
        }
        case OpCodeType::AssignRW: {
            throw "not implemented: eval AssignRW";
            break;
        }
        case OpCodeType::StartObj: {
            throw "not implemented: eval StartObj";
            break;
        }
        case OpCodeType::EndObj: {
            throw "not implemented: eval EndObj";
            break;
        }
        }
    }

    return stack.back();
}
