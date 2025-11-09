#include "oroba/eval.hpp"

#include <vector>

using namespace std;

OrobaObject* eval(Bytecode code, OrobaObject* init, LocalCollector& collector) {
    vector<OrobaObject*> activation_objects;
    activation_objects.push_back(init);

    vector<OrobaObject*> stack;

    for (auto& bc : code.ops) {
        switch (bc.type) {
        case OpCodeType::ImplMessage: {
            vector args(stack.end() - bc.message.num_operands, stack.end());
            stack.resize(stack.size() - bc.message.num_operands);
            activation_objects.back()->SendInternalMessage(bc.message.name, args, collector);
            break;
        }
        case OpCodeType::ExplMessage: {
            vector args(stack.end() - bc.message.num_operands, stack.end());
            stack.resize(stack.size() - bc.message.num_operands);
            OrobaObject* target = stack.back();
            stack.pop_back();
            target->SendExternalMessage(bc.message.name, args, collector);
            break;
        }
        case OpCodeType::Push: {
            stack.push_back(bc.literal);
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
        case OpCodeType::Nop: {
            break;
        }
        }
    }

    return stack.back();
}
