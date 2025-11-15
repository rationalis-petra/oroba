#include "oroba/world/lobby.hpp"

#include <iostream>

#include "oroba/object/primitives.hpp"
#include "oroba/object/multimethod.hpp"

using namespace std;

LobbyObject::LobbyObject(OrobaObject* math, OrobaObject* world, LocalCollector& collector) {
    Slot s = Slot {.parent_priority = 1, math};
    slots["math"] = s;
    s = Slot {.parent_priority = 1, world};
    slots["world"] = s;
}

std::string LobbyObject::Representation() {
    return "lobby";
}
