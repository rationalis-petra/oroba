#include "oroba/object/namespace.hpp"

using namespace std;

OrobaObject* NamespaceObject::SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    auto res = children.find(name);

    // TODO: check if is method?
    if (res != end(children)) {
        return res->second;
    }
    return MessageNotFound(name, args, collector);
}

void NamespaceObject::Trace() {
    tagged = true;
    for (auto& pr : children) {
        pr.second->Trace();
    }
}
