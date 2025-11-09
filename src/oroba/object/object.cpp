#include "oroba/object/object.hpp"

#include <string>

OrobaObject* OrobaObject::MessageNotFound(string name, vector<OrobaObject*> args) {
    throw std::string("message not found:" + name);
}
