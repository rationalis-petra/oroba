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

class OrobaObject : public Collectable {
public:
    virtual OrobaObject* SendExternalMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) = 0;
    virtual OrobaObject* SendInternalMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) = 0;

    OrobaObject* MessageNotFound(std::string name, std::vector<OrobaObject*> args);
};

#endif
