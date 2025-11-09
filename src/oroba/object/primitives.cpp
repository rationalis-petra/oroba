#include "oroba/object/primitives.hpp"

#include <sstream>

#include "oroba/object/collections.hpp"

using namespace std;

OrobaObject* PrimitiveObject::SendMessage(bool internal, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "to-string" && args.size() == 0) {
        OrobaObject* stringobj = new StringObject(ToString());
        collector.Add(stringobj);
        return stringobj;
    } else 
        return MessageNotFound(name, args, collector);
}

BooleanObject::BooleanObject(bool source) : val(source) {
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

