#ifndef __OROBA_WORLD_OS_HPP
#define __OROBA_WORLD_OS_HPP

#include "oroba/object/namespace.hpp"

class OSObject : public NamespaceObject {
public:
    OSObject(LocalCollector& collector);
    virtual std::string Representation () override;
};

#endif
