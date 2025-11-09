#ifndef __OROBA_OBJECT_COLLECTIONS_HPP
#define __OROBA_OBJECT_COLLECTIONS_HPP

#include <stdint.h>
#include <string>

#include "oroba/object/object.hpp"

class StringObject : public OrobaObject {
public:
    StringObject(std::string source);
    virtual OrobaObject* SendExternalMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& coll) override;
    virtual OrobaObject* SendInternalMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& coll) override;

public:
    std::string data;
};

#endif
