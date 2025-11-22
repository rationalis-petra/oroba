#ifndef __OROBA_OBJECT_NAMESPACE_HPP
#define __OROBA_OBJECT_NAMESPACE_HPP

#include <cstdint>
#include <unordered_map>
#include <string>

#include "oroba/objects/composite.hpp"

struct NamespaceObject : public CompositeObject {
    void PostAssignmentSetup(LocalCollector& collector);
};

#endif
