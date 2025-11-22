#ifndef __OROBA_OBJECT_PRIMITIVE_HPP
#define __OROBA_OBJECT_PRIMITIVE_HPP

#include <stdint.h>
#include <string>

#include "oroba/objects/object.hpp"

struct PrimitiveMethod {
    virtual ~PrimitiveMethod() = default;
    virtual OrobaObject* Invoke(std::vector<OrobaObject*> args, LocalCollector& collector) = 0;
};

class PrimitiveObject : public OrobaObject {
public:
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override;
    virtual OrobaObject* PrimitiveMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override;
    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() = 0;
};

#endif
