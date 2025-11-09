#ifndef __OROBA_OBJECT_PRIMITIVES_HPP
#define __OROBA_OBJECT_PRIMITIVES_HPP

#include <stdint.h>
#include <string>

#include "object/object.hpp"

class PrimitiveObject : public OrobaObject {
public:
    virtual OrobaObject SendExternalMessage(string name, vector<OraObject> args) override;
    virtual OrobaObject SendInternalMessage(string name, vector<OraObject> args) override;

    virtual string ToString() = 0;
}

class PrimBool : public PrimitiveObject {
public:
    virtual string ToString() override;

private:
    bool val;
};

class PrimInteger : public PrimitiveObject {
public:
    PrimInteger(int64_t val);
    virtual string ToString() override;

    // TODO: convert to big-int
    uint64_t val;
};

class PrimF32 : public PrimitiveObject {
public:
    PrimF32(float val);
    virtual string ToString() override;
    float val;
};

class PrimF64 : public PrimitiveObject {
public:
    PrimF64(double val);
    virtual string ToString() override;
    double val;
};

#endif
