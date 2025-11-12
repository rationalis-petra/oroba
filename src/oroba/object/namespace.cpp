#include "oroba/object/namespace.hpp"

using namespace std;

void NamespaceObject::AddMethod(std::string name, Method method) {
    methods[name] = method;
} 

void NamespaceObject::AddValue(std::string name, OrobaObject* value) {
    Slot slot{};
    slot.parent_priority = 0;
    slot.object = value;
    slots[name] = slot;

    SlotMethod m{false, name};
    methods[name] = Method(m);
    // add getter method;
}

void NamespaceObject::PostAssignmentSetup(LocalCollector& collector) {
    for (auto& pr : slots) {
        collector.Add(pr.second.object);
    }
    for (auto& pr : methods) {
        pr.second.AddToCollector(collector);
    }
}

