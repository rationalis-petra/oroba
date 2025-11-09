#include "oroba/object/primitives.hpp"

#include <sstream>

#include "oroba/object/collections.hpp"

using namespace std;

OrobaObject* PrimitiveObject::SendExternalMessage(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "to-string" && args.size() == 0) {
        OrobaObject* stringobj = new StringObject(ToString());
        collector.Add(stringobj);
        return stringobj;
    } else 
        return MessageNotFound(name, args);
}

OrobaObject* PrimitiveObject::SendInternalMessage(string name, vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "to-string" && args.size() == 0)
        return this;
    else 
        return MessageNotFound(name, args);
}


// class PrimBool : public PrimitiveObject {
// public:
//     virtual string ToString() override;

// private:
//     bool val;
// };

IntegerObject::IntegerObject(int64_t source) : val(source) {
}

string IntegerObject::ToString() {
    ostringstream oss;
    oss << val;
    return oss.str();
}

