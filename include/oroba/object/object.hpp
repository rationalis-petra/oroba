#ifndef __OROBA_OBJECT_OBJECT_HPP
#define __OROBA_OBJECT_OBJECT_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include "gc/collector.hpp"

using namespace std;

// ------------------------------------------------------------------------
//   Oroba Object 
// ---------
//
// ------------------------------------------------------------------------

class OrobaObject : public Collectable {
public:
    virtual OrobaObject* SendExternalMessage(string name, vector<OrobaObject*> args) = 0;
    virtual OrobaObject* SendInternalMessage(string name, vector<OrobaObject*> args) = 0;

    OrobaObject* MessageNotFound(string name, vector<OrobaObject*> args);
};

#endif
