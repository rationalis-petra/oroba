#ifndef __OROBA_WORLD_WORLD_HPP
#define __OROBA_WORLD_WORLD_HPP

#include "oroba/object/namespace.hpp"

class WorldObject : public NamespaceObject {
public:
    WorldObject(LocalCollector& collector);
};

#endif
