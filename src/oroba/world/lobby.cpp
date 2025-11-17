#include "oroba/world/lobby.hpp"

#include <iostream>

#include "oroba/world/traits.hpp"
#include "oroba/object/primitives.hpp"
#include "oroba/object/multimethod.hpp"

using namespace std;

LobbyNs::LobbyNs(OrobaObject* math, OrobaObject* world, LocalCollector& collector) {
    Slot s = Slot {.parent_priority = 1, math};
    slots["math"] = s;
    s = Slot {.parent_priority = 1, world};
    slots["world"] = s;
    s = Slot {.parent_priority = 2, TraitsNs::scope_trait};
    slots["parent"] = s;
}

std::string LobbyNs::Representation() {
    return "world lobby";
}
