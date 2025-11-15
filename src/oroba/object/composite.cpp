#include "oroba/data/error.hpp"
#include "oroba/eval.hpp"
#include "oroba/object/composite.hpp"

#include <unordered_set>

using namespace std;

optional<pair<CompositeObject*, Method>> CompositeObject::MethodLookup(std::string name) {
    auto res = methods.find(name);
    if (res != end(methods)) {
        return pair(this, res->second);
    } else {
        optional<pair<CompositeObject*, Method>> method = nullopt;
        for (auto slot : slots) {
            // TODO: priority
            if (slot.second.parent_priority > 0) {
                auto parent_method = slot.second.object->MethodLookup(name);
                if (method.has_value() && parent_method.has_value()) {
                    throw OrobaError("Ambiguous method resolution");
                } else if (!method.has_value()) {
                    method = parent_method;
                }
            }
        }
        return method;
    }
}

OrobaObject* CompositeObject::SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    // TODO: check for primitive message...
    auto res = MethodLookup(name);
    if (res.has_value()) {
        auto slot_holder = res.value().first;
        auto method = res.value().second;
        if (holds_alternative<SlotMethod>(method.method)) {
            // TODO: check if method is public/private!
            SlotMethod m = get<SlotMethod>(method.method);
            Slot s = slot_holder->slots[m.slot];
            if (m.is_setter) {
                // TODO: check for args.size() == 1
                s.object = args[0];
                slot_holder->slots[m.slot] = s;
                return this;
            } else {
                // TODO: check for args.size() == 0
                return s.object;
            }
        } else if (holds_alternative<PrimitiveMethod*>(method.method)) {
            PrimitiveMethod* prim = get<PrimitiveMethod*>(method.method);
            prim->Invoke(args, collector);
        } else {
            UserMethod user = get<UserMethod>(method.method);
            CompositeObject* activationObject = new CompositeObject;
            collector.Add(activationObject);

            Slot parent{};
            parent.parent_priority = 1;
            parent.object = this;
            activationObject->slots["parent"] = parent;

            if (user.args.size() != args.size()) {
                throw OrobaError("incorrect number of arguments to user method.");
            }

            for (size_t i = 0; i < args.size(); i++) {
                Slot argslot{};
                parent.parent_priority = 1;
                argslot.object = args[i];
                activationObject->slots[user.args[i]] = argslot;
            }
            
            return eval(user.body, activationObject, collector);
        }
    }

    // TODO: work our way up parent slots.

    return OrobaObject::SendMessage(internal, name, args, collector);
}
void CompositeObject::AddMethod(std::string name, Method method) {
    methods[name] = method;
} 

void CompositeObject::AddValue(std::string name, OrobaObject* value) {
    Slot slot{};
    slot.parent_priority = 0;
    slot.object = value;
    slots[name] = slot;

    SlotMethod m{false, name};
    methods[name] = Method(m);
    // add getter method;
}


void CompositeObject::Trace() {
    if (tagged) return;
    tagged = true;

    for (auto slot : slots) {
        slot.second.object->Trace();
    }
    for (auto method : methods) {
        method.second.Trace();
    }
}


std::string CompositeObject::Representation() {
    return "use object";
}

