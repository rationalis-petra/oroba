#include "oroba/objects/object.hpp"

#include "oroba/eval.hpp"
#include "oroba/data/error.hpp"
#include "oroba/objects/collections.hpp"

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

Method::Method() : method(UserMethod()) { } 
Method::Method(SlotMethod slot_method) : method(slot_method) { }
Method::Method(UserMethod user_method) : method(user_method) { }

void Method::Trace() {
    // TODO: trace through code if user method? 
}
