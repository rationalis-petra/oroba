#ifndef __OROBA_WORLD_SYSTEM_HPP
#define __OROBA_WORLD_SYSTEM_HPP

#include "oroba/object/namespace.hpp"

class SystemObject : public NamespaceObject {
public:
    SystemObject(LocalCollector& collector);
    virtual std::string Representation() override;
};

#endif
