#include "oroba/object/collections.hpp"

#include <sstream>

using namespace std;

StringObject::StringObject(string source) : data(source) { }

OrobaObject* StringObject::SendMessage(bool internal, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    return OrobaObject::SendMessage(internal, name, args, collector);
}

std::string StringObject::Representation() {
    return data;
}

