#ifndef __OROBA_OBJECT_OBJECT_HPP
#define __OROBA_OBJECT_OBJECT_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include "gc/collector.hpp"

// ------------------------------------------------------------------------
//   Oroba Object 
// ---------
//
// ------------------------------------------------------------------------

enum class Visibility {Public, Private};

struct SlotDescriptor {
    Visibility read_visibility;
    Visibility write_visibility;
    bool can_write;
    bool is_argslot;
    int parent_priority;
};

class OrobaObject : public Collectable, public Tracer {
public:
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector);
    virtual OrobaObject* Evaluate(LocalCollector& collector);

    OrobaObject* MessageNotFound(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector);

    // Tracer interface
    virtual void Trace() override; 

protected:
    std::unordered_map<std::string, OrobaObject*> slots;
};

class MethodObject : public OrobaObject {
public:
    virtual OrobaObject* MakeActivationObject(std::vector<OrobaObject*> args, LocalCollector& collector) = 0;
};

#endif
