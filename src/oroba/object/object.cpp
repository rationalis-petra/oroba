#include "oroba/object/object.hpp"

using namespace std;

OrobaObject* OrobaObject::MessageNotFound(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    throw std::string("message not found: " + name);
}

void OrobaObject::Trace() {
    tagged = true;
}
