#include "oroba/world/world.hpp"

#include "oroba/object/primitives.hpp"
#include "oroba/world/ports.hpp"

using namespace std;

WorldObject::WorldObject(LocalCollector& collector) {
    children["true"] = new BooleanObject(true);
    children["false"] = new BooleanObject(false);

    children["ports"] = new PortsObject(collector);

    for (auto& pr : children) {
        collector.Add(pr.second);
    }
}
