#ifndef __OROBA_OBJECT_COLLECTIONS_HPP
#define __OROBA_OBJECT_COLLECTIONS_HPP

#include <stdint.h>
#include <string>

#include "oroba/objects/primitive.hpp"

class StringObject : public PrimitiveObject {
public:
    StringObject(std::string source);
    virtual std::string Representation() override;
    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() override;

    static void Init();
    static void Teardown();

public:
    std::string data;
};

#endif
