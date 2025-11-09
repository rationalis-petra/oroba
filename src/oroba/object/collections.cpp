#include "oroba/object/collections.hpp"

using namespace std;

StringObject::StringObject(string source) : data(source) { }

OrobaObject* StringObject::SendExternalMessage(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "to-string" && args.size() == 0)
        return this;
    else 
        return MessageNotFound(name, args);
}


OrobaObject* StringObject::SendInternalMessage(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "to-string" && args.size() == 0)
        return this;
    else 
        return MessageNotFound(name, args);
}
