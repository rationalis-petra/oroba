#include "oroba/object/collections.hpp"

#include <sstream>

using namespace std;

StringObject::StringObject(string source) : data(source) { }

static OrobaObject* MessageResponse(StringObject* obj, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "to-string" && args.size() == 0) {
        return obj;
    }
    else 
        return obj->MessageNotFound(name, args, collector);
}

OrobaObject* StringObject::SendExternalMessage(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    return MessageResponse(this, name, args, collector);
}


OrobaObject* StringObject::SendInternalMessage(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    return MessageResponse(this, name, args, collector);
}
