#ifndef __OROBA_WORLD_LOBBY_HPP
#define __OROBA_WORLD_LOBBY_HPP

#include "oroba/object/namespace.hpp"

class LobbyNs : public NamespaceObject {
public:
    LobbyNs(OrobaObject* math, OrobaObject* world, LocalCollector& collector);
    virtual std::string Representation() override;
};

#endif
