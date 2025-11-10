#include "oroba/object/object.hpp"

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
