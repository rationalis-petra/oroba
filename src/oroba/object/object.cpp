#include "oroba/object/object.hpp"

#include "oroba/eval.hpp"
#include "oroba/data/error.hpp"
#include "oroba/object/collections.hpp"

using namespace std;

OrobaObject* OrobaObject::SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    // TODO: remove non-primitive 'to-string'
    if (name == "_representation" || name == "to-string") {
        OrobaObject* out = new StringObject(Representation());
        collector.Add(out);
        return out;
    } else {
        return MessageNotFound(name, args, collector);
    }
}

optional<pair<CompositeObject*, Method>> OrobaObject::MethodLookup(string name) {
    return nullopt;
}

OrobaObject* OrobaObject::MessageNotFound(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    string msg = "message not found: " + name + " in object " + Representation();
    throw OrobaError(msg);
}

void OrobaObject::Trace() {
    tagged = true;
}

Method::Method() : method(nullptr) { } 
Method::Method(SlotMethod slot_method) : method(slot_method) { }
Method::Method(UserMethod user_method) : method(user_method) { }
Method::Method(PrimitiveMethod* prim_method) : method(prim_method) { }

void Method::Trace() {
    if (tagged) return;
    tagged = true;
    if (holds_alternative<PrimitiveMethod*>(method)) {
        PrimitiveMethod* prim = get<PrimitiveMethod*>(method);
        prim->tagged = true;
    }
}

void Method::AddToCollector(LocalCollector& collector) {
    if (holds_alternative<PrimitiveMethod*>(method)) {
        collector.Add(get<PrimitiveMethod*>(method));
    }
}
