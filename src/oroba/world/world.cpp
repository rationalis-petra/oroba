#include "oroba/world/world.hpp"

#include "oroba/object/primitives.hpp"

using namespace std;

WorldObject::WorldObject(LocalCollector& collector) {
    children["true"] = new BooleanObject(true);
    children["false"] = new BooleanObject(false);

    for (auto& pr : children) {
        collector.Add(pr.second);
    }
}

OrobaObject* WorldObject::SendExternalMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    auto res = children.find(name);

    // TODO: check if is method?
    if (res != end(children)) {
        return res->second;
    }
    return MessageNotFound(name, args, collector);
}

OrobaObject* WorldObject::SendInternalMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    auto res = children.find(name);

    // TODO: check if is method?
    if (res != end(children)) {
        return res->second;
    }
    return MessageNotFound(name, args, collector);
}

void WorldObject::Trace() {
    tagged = true;
    for (auto& pr : children) {
        pr.second->Trace();
    }
}
