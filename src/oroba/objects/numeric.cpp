#include "oroba/objects/numeric.hpp"

#include <sstream>

#include "oroba/objects/collections.hpp"

using namespace std;

NilObject* NilObject::nil = nullptr;

string NilObject::Representation() { return "nil"; }

static unordered_map<string, PrimitiveMethod*> nil_primitives;
std::unordered_map<std::string, PrimitiveMethod*> NilObject::GetPrimitives() {
    return nil_primitives;
}

void NilObject::Init() {
    //NilObject::nil = new NilObject();
}

void NilObject::Teardown() {
    for (auto& pr : nil_primitives) {
        delete pr.second;
    }
}


BooleanObject::BooleanObject(bool source) : val(source) {}

static unordered_map<string, PrimitiveMethod*> bool_primitives;
std::unordered_map<std::string, PrimitiveMethod*> BooleanObject::GetPrimitives() {
    return bool_primitives;
}

string BooleanObject::Representation() {
    return val ? "true" : "false";
}

void BooleanObject::Init() {
}

void BooleanObject::Teardown() {
    for (auto& pr : bool_primitives) {
        delete pr.second;
    }
}



IntegerObject::IntegerObject(int64_t source) : val(source) {
}

static unordered_map<string, PrimitiveMethod*> int64_primitives;
std::unordered_map<std::string, PrimitiveMethod*> IntegerObject::GetPrimitives() {
    return int64_primitives;
}

string IntegerObject::Representation() {
    ostringstream oss;
    oss << val;
    return oss.str();
}

void IntegerObject::Init() {
}

void IntegerObject::Teardown() {
}

SymbolObject::SymbolObject(string sym) : symbol (sym) { }

string SymbolObject::Representation() {
    return "#" + symbol;
}

static unordered_map<string, PrimitiveMethod*> symbol_primitives;

std::unordered_map<std::string, PrimitiveMethod*> SymbolObject::GetPrimitives() {
    return symbol_primitives;
}

void SymbolObject::Init() {
    //NilObject::nil = new NilObject();
}

void SymbolObject::Teardown() {
    for (auto& pr : symbol_primitives) {
        delete pr.second;
    }
}
