#ifndef __OROBA_WORLD_LOBBY_HPP
#define __OROBA_WORLD_LOBBY_HPP

#include "oroba/object/namespace.hpp"

class LobbyObject : public NamespaceObject {
public:
    LobbyObject(OrobaObject* math, OrobaObject* world, LocalCollector& collector);
    virtual std::string Representation() override;
};

#endif
