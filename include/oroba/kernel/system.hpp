#ifndef __OROBA_WORLD_SYSTEM_HPP
#define __OROBA_WORLD_SYSTEM_HPP

#include "oroba/objects/namespace.hpp"

class SystemNs : public NamespaceObject {
public:
    SystemNs(LocalCollector& collector);
    virtual std::string Representation() override;
};

#endif
