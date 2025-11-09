#include "oroba/object/object.hpp"

#include "oroba/data/error.hpp"
#include "oroba/object/collections.hpp"

using namespace std;

OrobaObject* OrobaObject::MessageNotFound(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    string msg = "message not found: " + name;
    throw OrobaError(msg);
}

void OrobaObject::Trace() {
    tagged = true;
}
