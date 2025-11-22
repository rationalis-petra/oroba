#include "oroba/objects/system/ostream.hpp"

#include <ostream>

#include "oroba/data/error.hpp"
#include "oroba/objects/collections.hpp"
#include "oroba/objects/primitive_method.hpp"

using namespace std;

static unordered_map<string, PrimitiveMethod*> ostream_methods;

void OStreamObject::Init() {
    PrimitiveMethod* method;
    method = new TypedPrimitiveMethod<OStreamObject*, StringObject*>(
      "write-string:",
      [](OStreamObject* stream, StringObject* string, LocalCollector& coll) {
          stream->ostream << string->data;
          return stream;
    });
    ostream_methods["write-string:"] = method;
}

void OStreamObject::Teardown() {
    for (auto& pr : ostream_methods) {
        delete pr.second;
    }
}

unordered_map<string, PrimitiveMethod*> OStreamObject::GetPrimitives() {
    return ostream_methods;
}

OStreamObject::OStreamObject(std::ostream& _ostream) : ostream(_ostream) {}

string OStreamObject::Representation() {
    return "<output stream primitive>";
}
