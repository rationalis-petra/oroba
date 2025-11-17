#include "oroba/object/namespace.hpp"

#include "oroba/world/traits.hpp"

using namespace std;

void NamespaceObject::PostAssignmentSetup(LocalCollector& collector) {
    Slot trait_slot;
    trait_slot.parent_priority = 1;
    trait_slot.object = TraitsNs::scope_trait;
    slots["parent"] = trait_slot;

    for (auto& pr : slots) {
        collector.Add(pr.second.object);
    }
    for (auto& pr : methods) {
        pr.second.AddToCollector(collector);
    }
}

