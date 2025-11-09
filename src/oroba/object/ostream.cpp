#include "oroba/object/ostream.hpp"

#include <ostream>

#include "oroba/data/error.hpp"
#include "oroba/object/collections.hpp"

using namespace std;

OStreamObject::OStreamObject(std::ostream& _ostream) : ostream(_ostream) {}

OrobaObject* OStreamObject::SendMessage(bool internal, string name, vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "write:" && args.size() == 1) {
        OrobaObject* arg = args[0];
        OrobaObject* result_tostr = arg->SendMessage(false, "to-string", vector<OrobaObject*>(), collector);
        StringObject* str = dynamic_cast<StringObject*>(result_tostr);
        if (str) {
            ostream << str->data;
            return this;
        } else {
            throw OrobaError("Object did not convert to string; threfore cannot write to ostream.");
        }
    } else if (name == "write-ln:" && args.size() == 1) {
        OrobaObject* arg = args[0];
        OrobaObject* result_tostr = arg->SendMessage(false, "to-string", vector<OrobaObject*>(), collector);
        StringObject* str = dynamic_cast<StringObject*>(result_tostr);
        if (str) {
            ostream << str->data << "\n";
            return this;
        } else {
            throw OrobaError("Object did not convert to string; threfore cannot write to ostream.");
        }
    } else if (name == "to-string" && args.size() == 0) {
        OrobaObject* out = new StringObject("*standard output port*");
        collector.Add(out);
        return out;
    } else  {
        return MessageNotFound(name, args, collector);
    }
}
