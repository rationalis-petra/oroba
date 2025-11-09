#include "oroba/object/object.hpp"

using namespace std;

OrobaObject* OrobaObject::MessageNotFound(string name, vector<OrobaObject*> args) {
    throw std::string("message not found:" + name);
}
