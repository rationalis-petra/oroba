#ifndef __OROBA_WORLD_WORLD_HPP
#define __OROBA_WORLD_WORLD_HPP

#include "oroba/objects/namespace.hpp"

class WorldObject : public NamespaceObject {
public:
    WorldObject(LocalCollector& collector);
    virtual std::string Representation() override;
};

#endif
