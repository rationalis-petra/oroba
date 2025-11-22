#ifndef __OROBA_WORLD_TRAITS_HPP
#define __OROBA_WORLD_TRAITS_HPP

#include "oroba/objects/namespace.hpp"

class TraitsNs : public NamespaceObject {
public:
    TraitsNs(LocalCollector& collector);
    virtual std::string Representation() override;

    static OrobaObject* scope_trait;
    static OrobaObject* bool_trait;
};

#endif
