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

class OrobaObject : public Collectable, public Tracer {
public:
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) = 0;

    OrobaObject* MessageNotFound(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector);

    // Tracer interface
    virtual void Trace() override; 
};

#endif
