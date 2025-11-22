#ifndef __OROBA_WORLD_GENERICS_HPP
#define __OROBA_WORLD_GENERICS_HPP

#include "oroba/objects/namespace.hpp"

class GenericsNs : public NamespaceObject {
public:
    GenericsNs(LocalCollector& collector);
    virtual std::string Representation() override;
};

#endif
