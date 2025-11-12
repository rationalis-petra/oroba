#ifndef __OROBA_OBJECT_COMPOSITE_HPP
#define __OROBA_OBJECT_COMPOSITE_HPP

#include <vector>
#include <vector>
#include <string>
#include <variant>
#include <unordered_map>

#include "gc/collector.hpp"
#include "oroba/data/bytecode.hpp"
#include "oroba/object/object.hpp"

// ------------------------------------------------------------------------
//   Oroba Object 
// ---------
//
// ------------------------------------------------------------------------

struct Slot {
    int parent_priority;
    OrobaObject* object;
};

class CompositeObject : public OrobaObject {
public:
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override;
    virtual std::optional<std::pair<CompositeObject*, Method>> MethodLookup(std::string name) override;

    // Tracer interface
    virtual void Trace() override; 

    std::unordered_map<std::string, Slot> slots;
    std::unordered_map<std::string, Method> methods;
};


#endif
