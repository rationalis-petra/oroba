#ifndef __OROBA_OBJECT_OBJECT_HPP
#define __OROBA_OBJECT_OBJECT_HPP

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "gc/collector.hpp"
#include "oroba/data/bytecode.hpp"

// ------------------------------------------------------------------------
//   Oroba Object 
// ---------
//
// ------------------------------------------------------------------------

class OrobaObject : public Collectable, public Tracer {
public:
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector);
    virtual OrobaObject* Evaluate(LocalCollector& collector);

    OrobaObject* MessageNotFound(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector);

    // Tracer interface
    virtual void Trace() override; 

    std::unordered_map<std::string, OrobaObject*> slots;
};

class MethodObject : public OrobaObject {
public:
    virtual OrobaObject* MakeActivationObject(std::vector<OrobaObject*> args, LocalCollector& collector) = 0;
};

class UserObject : public OrobaObject {
public:
    virtual OrobaObject* Evaluate(LocalCollector& collector) override;

    // Tracer interface
    virtual void Trace() override; 

    std::shared_ptr<Bytecode> code = nullptr;
};

#endif
