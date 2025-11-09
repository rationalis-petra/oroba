#ifndef __OROBA_OBJECT_COLLECTIONS_HPP
#define __OROBA_OBJECT_COLLECTIONS_HPP

#include <stdint.h>
#include <string>

#include "oroba/object/object.hpp"

class StringObject : public OrobaObject {
public:
    StringObject(string source);

    virtual OrobaObject* SendExternalMessage(string name, vector<OrobaObject*> args) override;
    virtual OrobaObject* SendInternalMessage(string name, vector<OrobaObject*> args) override;

public:
    string data;
};

#endif
