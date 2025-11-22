#include "oroba/objects/collections.hpp"

#include <sstream>

using namespace std;

StringObject::StringObject(string source) : data(source) { }

std::string StringObject::Representation() {
    return data;
}

static unordered_map<string, PrimitiveMethod*> string_primitives;

std::unordered_map<std::string, PrimitiveMethod*> StringObject::GetPrimitives() {
    return string_primitives;
}

void StringObject::Init() {
}

void StringObject::Teardown() {
    for (auto& pr : string_primitives) {
        delete pr.second;
    }
}
