#include "oroba/object/collections.hpp"

StringObject::StringObject(string source) : data(source) { }

OrobaObject* StringObject::SendExternalMessage(string name, vector<OrobaObject*> args) {
    if (name == "to-string" && args.size() == 0)
        return this;
    else 
        return MessageNotFound(name, args);
}


OrobaObject* StringObject::SendInternalMessage(string name, vector<OrobaObject*> args) {
    if (name == "to-string" && args.size() == 0)
        return this;
    else 
        return MessageNotFound(name, args);
}
