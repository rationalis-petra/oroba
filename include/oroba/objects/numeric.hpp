#ifndef __OROBA_OBJECT_NUMERIC_HPP
#define __OROBA_OBJECT_NUMERIC_HPP

#include <stdint.h>
#include <string>

#include "oroba/objects/object.hpp"
#include "oroba/objects/primitive.hpp"

class NilObject : public PrimitiveObject {
public:
    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() override;
    virtual std::string Representation() override;

    static void Init();
    static void Teardown();
    static NilObject* nil;
};

class BooleanObject : public PrimitiveObject {
public:
    BooleanObject(bool source);
    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() override;
    virtual std::string Representation() override;

    static void Init();
    static void Teardown();

private:
    bool val;
};

class IntegerObject : public PrimitiveObject {
public:
    IntegerObject(int64_t source);
    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() override;
    virtual std::string Representation() override;

    static void Init();
    static void Teardown();
    uint64_t val;
};

class PrimF32 : public PrimitiveObject {
public:
    PrimF32(float val);
    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() override;
    virtual std::string Representation() override;

    static void Init();
    static void Teardown();
    float val;
};

class PrimF64 : public PrimitiveObject {
public:
    PrimF64(double val);
    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() override;
    virtual std::string Representation() override;

    static void Init();
    static void Teardown();
    double val;
};

class SymbolObject : public PrimitiveObject {
public:
    SymbolObject(std::string sym);
    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() override;
    virtual std::string Representation() override;

    static void Init();
    static void Teardown();
    std::string symbol;
};

#endif
