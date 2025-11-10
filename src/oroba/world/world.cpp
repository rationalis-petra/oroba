#include "oroba/world/world.hpp"

#include "oroba/object/primitives.hpp"
#include "oroba/world/system.hpp"
#include "oroba/world/os.hpp"

using namespace std;

WorldObject::WorldObject(LocalCollector& collector) {
    slots["true"] = new BooleanObject(true);
    slots["false"] = new BooleanObject(false);

    slots["os"] = new OSObject(collector);
    slots["system"] = new SystemObject(collector);

    for (auto& pr : slots) {
        collector.Add(pr.second);
    }
}
