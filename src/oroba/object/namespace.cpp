#include "oroba/object/namespace.hpp"

using namespace std;

void NamespaceObject::PostAssignmentSetup(LocalCollector& collector) {
    for (auto& pr : slots) {
        collector.Add(pr.second.object);
    }
    for (auto& pr : methods) {
        pr.second.AddToCollector(collector);
    }
}

