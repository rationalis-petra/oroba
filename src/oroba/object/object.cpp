#include "oroba/object/object.hpp"

#include "oroba/eval.hpp"
#include "oroba/data/error.hpp"
#include "oroba/object/collections.hpp"

using namespace std;

OrobaObject* OrobaObject::SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    // TODO: respond to primitive messages.
    return MessageNotFound(name, args, collector);
}

optional<pair<CompositeObject*, Method>> OrobaObject::MethodLookup(string name) {
    return nullopt;
}

OrobaObject* OrobaObject::MessageNotFound(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    string msg = "message not found: " + name;
    throw OrobaError(msg);
}

void OrobaObject::Trace() {
    tagged = true;
}
