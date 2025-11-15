#ifndef __OROBA_OBJECT_OBJECT_HPP
#define __OROBA_OBJECT_OBJECT_HPP

#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <unordered_map>

#include "gc/collector.hpp"
#include "oroba/data/bytecode.hpp"

// ------------------------------------------------------------------------
//   Oroba Object 
// ---------
//
// ------------------------------------------------------------------------

struct PrimitiveMethod : public Collectable {
    virtual OrobaObject* Invoke(std::vector<OrobaObject*> args, LocalCollector& collector) = 0;
};

struct SlotMethod {
    bool is_setter;
    std::string slot;
};

struct UserMethod {
    std::vector<std::string> args;
    Bytecode body;
};

struct Method : public Tracer, Collectable {
    Method();
    Method(SlotMethod slot_method);
    Method(UserMethod user_method);
    Method(PrimitiveMethod* prim_method);

    std::variant<SlotMethod, UserMethod, PrimitiveMethod*> method;
    virtual void Trace() override;
    virtual void AddToCollector(LocalCollector& collector);
};

class CompositeObject;

class OrobaObject : public Collectable, public Tracer {
public:
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector);
    virtual std::optional<std::pair<CompositeObject*, Method>> MethodLookup(std::string name);
    OrobaObject* MessageNotFound(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector);

    // Messages supported by all objects:
    virtual std::string Representation() = 0;

    // Tracer interface
    virtual void Trace() override; 
};

#endif
