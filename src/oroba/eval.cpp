#include "oroba/eval.hpp"

#include <vector>

#include "oroba/data/error.hpp"
#include "oroba/object/primitives.hpp"
#include "oroba/object/composite.hpp"

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
        case OpCodeType::Swap: {
            OrobaObject* ob1 = stack.back();
            stack.pop_back();
            OrobaObject* ob2 = stack.back();
            stack.pop_back();
            stack.push_back(ob1);
            stack.push_back(ob2);
            break;
        }
        case OpCodeType::MakeBlock: {
            MakeBlock mb = get<MakeBlock>(op.payload);
            CompositeObject* obj = new CompositeObject;
            // for (auto sldesc : mo.slots) {
            //     SlotDescriptor desc = sldesc.second;
            //     string name = sldesc.first;
            //     Slot slot;
            //     if (desc.) {
            //         slot.parent_priority = desc.parent_priority;
            //         slot.object = NilObject::nil;
            //         obj->slots[name] = slot;

            //         // Slot methods
            //         SlotMethod m{false, name};
            //         obj->methods[name] = Method(m);

            //         if (desc.can_write) {
            //             string settername = name + ":";
            //             SlotMethod m{true, name};
            //             obj->methods[settername] = Method(m);
            //         }
            //     }
            // }
            // for (auto to_init : mo.to_initialize) {
            //     obj->slots[to_init].object = stack.back();
            //     stack.pop_back();
            // }
            UserMethod method;
            method.args = mb.args;
            method.body = *mb.body;
            if (mb.args.size() == 0) {
                obj->methods["invoke"] = Method(method);
            } else {
                obj->methods["invoke:"] = Method(method);
            }
            collector.Add(obj);
            stack.push_back(obj);
            break;
        }
        case OpCodeType::MakeObject: {
            MakeObject mo = get<MakeObject>(op.payload);
            CompositeObject* obj = new CompositeObject;
            for (auto sldesc : mo.slots) {
                SlotDescriptor desc = sldesc.second;
                string name = sldesc.first;
                Slot slot;
                slot.parent_priority = desc.parent_priority;
                slot.object = NilObject::nil;
                obj->slots[name] = slot;

                // Slot methods
                SlotMethod m{false, name};
                obj->methods[name] = Method(m);

                if (desc.can_write) {
                    string settername = name + ":";
                    SlotMethod m{true, name};
                    obj->methods[settername] = Method(m);
                }
            }
            for (auto to_init : mo.to_initialize) {
                obj->slots[to_init].object = stack.back();
                stack.pop_back();
            }
            for (auto sldesc : mo.methods) {
                MethodDescriptor desc = sldesc.second;
                UserMethod method;
                method.args = desc.args;
                method.body = *desc.code;
                obj->methods[sldesc.first] = method;
            }
            // TODO: add methods

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
