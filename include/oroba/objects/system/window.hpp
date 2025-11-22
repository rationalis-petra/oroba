#ifndef __OROBA_OBJECT_SYTEM_WINDOW_HPP
#define __OROBA_OBJECT_SYTEM_WINDOW_HPP

#include <stdint.h>
#include <string>

#include "oroba/objects/object.hpp"

class StringObject : public OrobaObject {
public:
    StringObject(std::string source);
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& coll) override;
    virtual std::string Representation() override;

public:
    std::string data;
};

#endif
