#include "oroba/object/primitives.hpp"

#include <sstream>

#include "oroba/object/collections.hpp"

using namespace std;

OrobaObject* PrimitiveObject::SendMessage(bool internal, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "to-string") {
        // TODO - check for correct number of args. 
        OrobaObject* stringobj = new StringObject(ToString());
        collector.Add(stringobj);
        return stringobj;
    } else {
        return SendMessage(internal, name, args, collector);
    }
}

BooleanObject::BooleanObject(bool source) : val(source) {}

OrobaObject* BooleanObject::SendMessage(bool internal, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    // TODO: check for 2 args!
    if (name == "ifTrue:ifFalse:") {
        if (val) {
            return args[0]->SendMessage(false, "call", vector<OrobaObject*>{}, collector);
        } else {
            return args[1]->SendMessage(false, "call", vector<OrobaObject*>{}, collector);
        }
    } else {
        return SendMessage(internal, name, args, collector);
    }
}

string BooleanObject::ToString() {
    return val ? "true" : "false";
}


IntegerObject::IntegerObject(int64_t source) : val(source) {
}

string IntegerObject::ToString() {
    ostringstream oss;
    oss << val;
    return oss.str();
}

