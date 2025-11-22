#ifndef __OROBA_WORLD_MATH_HPP
#define __OROBA_WORLD_MATH_HPP

#include "oroba/objects/namespace.hpp"

class MathObject : public NamespaceObject {
public:
    MathObject(LocalCollector& collector);
    virtual std::string Representation() override;
};

#endif
