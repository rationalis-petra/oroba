#include "oroba/object/object.hpp"

#include "oroba/eval.hpp"
#include "oroba/data/error.hpp"
#include "oroba/object/collections.hpp"

using namespace std;

OrobaObject* OrobaObject::SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    auto res = slots.find(name);

    // TODO: check if is method?
    if (res != end(slots)) {
        OrobaObject* value = res->second;
        MethodObject* method = dynamic_cast<MethodObject*>(value);
        if (method) {
            OrobaObject* acitvation = method->MakeActivationObject(args, collector);
            return acitvation->Evaluate(collector);
        } else {
            return value;
        }
    }

    // TODO: better way of mutating slot values?
    if (name.back() == ':') {
        string slotname = name.substr(0, name.size() - 1); 
        auto res2 = slots.find(slotname);
        if (res2 != end(slots)) {
            // TODO: check for 1 arg
            slots[slotname] = args[0];
            return this;
        }
    }

    return MessageNotFound(name, args, collector);
}

OrobaObject* OrobaObject::MessageNotFound(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    string msg = "message not found: " + name;
    throw OrobaError(msg);
}

OrobaObject* OrobaObject::Evaluate(LocalCollector& collector) {
    return this;
}

void OrobaObject::Trace() {
    tagged = true;
    for (auto& pr : slots) {
        pr.second->Trace();
    }
}

OrobaObject* UserObject::Evaluate(LocalCollector& collector) {
    if (code == nullptr) {
        return this;
    } else {
        return eval(*code, this, collector);
    }
}

void UserObject::Trace() {
    OrobaObject::Trace();
    if (code) { code->Trace(); }
}
