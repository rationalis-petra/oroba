#ifndef __OROBA_WORLD_TRAITS_HPP
#define __OROBA_WORLD_TRAITS_HPP

#include "oroba/object/namespace.hpp"

class TraitsNs : public NamespaceObject {
public:
    TraitsNs(LocalCollector& collector);
    virtual std::string Representation() override;

    static OrobaObject* bool_trait;
};

#endif
