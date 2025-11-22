#include "oroba/objects/primitive.hpp"

using namespace std;

OrobaObject* PrimitiveObject::SendMessage(bool internal, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    return OrobaObject::SendMessage(internal, name, args, collector);
}

OrobaObject* PrimitiveObject::PrimitiveMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    auto map = GetPrimitives();
    auto it = map.find(name);
    if (it != map.end()) {
        PrimitiveMethod* m = it->second;
        return m->Invoke(args, collector);
    } else {
        return MessageNotFound(name, args, collector);
    }
}

