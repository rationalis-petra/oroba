#ifndef __OROBA_OBJECT_PRIMITIVES_HPP
#define __OROBA_OBJECT_PRIMITIVES_HPP

#include <stdint.h>
#include <string>

#include "oroba/object/object.hpp"

class PrimitiveObject : public OrobaObject {
public:
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override;
};

class NilObject : public PrimitiveObject {
public:
    virtual std::string Representation() override;
    static NilObject* nil;
};

class BooleanObject : public PrimitiveObject {
public:
    BooleanObject(bool source);
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override;
    virtual std::string Representation() override;

private:
    bool val;
};

class IntegerObject : public PrimitiveObject {
public:
    IntegerObject(int64_t source);
    virtual std::string Representation() override;

    // TODO: convert to big-int
    uint64_t val;
};

class PrimF32 : public PrimitiveObject {
public:
    PrimF32(float val);
    virtual std::string Representation() override;
    float val;
};

class PrimF64 : public PrimitiveObject {
public:
    PrimF64(double val);
    virtual std::string Representation() override;
    double val;
};

#endif
