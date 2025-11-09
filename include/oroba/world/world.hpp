#ifndef __OROBA_WORLD_WORLD_HPP
#define __OROBA_WORLD_WORLD_HPP

#include <cstdint>
#include <unordered_map>
#include <string>

#include "oroba/object/object.hpp"

class WorldObject : public OrobaObject {
public:
    WorldObject(LocalCollector& collector);

    virtual OrobaObject* SendExternalMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& coll) override;
    virtual OrobaObject* SendInternalMessage(std::string name, std::vector<OrobaObject*> args, LocalCollector& coll) override;
    
    virtual void Trace() override; 

private:
    std::unordered_map<std::string, OrobaObject*> children;
};

#endif
