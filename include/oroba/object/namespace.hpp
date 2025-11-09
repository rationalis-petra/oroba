#ifndef __OROBA_OBJECT_NAMESPACE_HPP
#define __OROBA_OBJECT_NAMESPACE_HPP

#include <cstdint>
#include <unordered_map>
#include <string>

#include "oroba/object/object.hpp"

class NamespaceObject : public OrobaObject {
public:
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& coll) override;
    
    virtual void Trace() override; 

protected:
    std::unordered_map<std::string, OrobaObject*> children;
};

#endif
