#include "oroba/object/collections.hpp"

#include <sstream>

using namespace std;

StringObject::StringObject(string source) : data(source) { }

OrobaObject* StringObject::SendMessage(bool internal, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "to-string" && args.size() == 0) {
        return this;
    }
    else {
        return MessageNotFound(name, args, collector);
    }
}


