#ifndef __OROBA_OBJECT_OSTREAM_HPP
#define __OROBA_OBJECT_OSTREAM_HPP

#include <ostream>
#include "oroba/objects/primitive.hpp"

class OStreamObject : public PrimitiveObject {
public:
    OStreamObject(std::ostream& ostream);
    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() override;
    virtual std::string Representation();

    static void Init();
    static void Teardown();

    std::ostream& ostream;
};

#endif
