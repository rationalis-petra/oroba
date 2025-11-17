#include "oroba/object/primitives.hpp"

#include <sstream>

#include "oroba/object/collections.hpp"

using namespace std;

OrobaObject* PrimitiveObject::SendMessage(bool internal, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    return OrobaObject::SendMessage(internal, name, args, collector);
}

NilObject* NilObject::nil = nullptr;

string NilObject::Representation() { return "nil"; }

BooleanObject::BooleanObject(bool source) : val(source) {}

OrobaObject* BooleanObject::SendMessage(bool internal, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    // TODO: check for 2 args!
    if (name == "ifTrue:ifFalse:") {
        if (val) {
            return args[0]->SendMessage(false, "invoke", vector<OrobaObject*>{}, collector);
        } else {
            return args[1]->SendMessage(false, "invoke", vector<OrobaObject*>{}, collector);
        }
    } 

    return PrimitiveObject::SendMessage(internal, name, args, collector);
}

string BooleanObject::Representation() {
    return val ? "true" : "false";
}


IntegerObject::IntegerObject(int64_t source) : val(source) {
}

string IntegerObject::Representation() {
    ostringstream oss;
    oss << val;
    return oss.str();
}

SymbolObject::SymbolObject(string sym) : symbol (sym) { }

string SymbolObject::Representation() {
    return "#" + symbol;
}
