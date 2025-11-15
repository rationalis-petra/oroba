#include "oroba/world/system.hpp"

#include <iostream>

#include "oroba/object/primitives.hpp"
#include "oroba/object/primitive_method.hpp"

using namespace std;

SystemObject::SystemObject(LocalCollector& collector) {
    AddMethod("exit", Method(new TypedPrimitiveMethod<>(
      "exit",
      [](LocalCollector& collector) -> OrobaObject* { exit(0); })
    ));

    AddMethod("exit:", new TypedPrimitiveMethod<IntegerObject*>(
      "exit:",
      [](IntegerObject* obj, LocalCollector& collector) -> OrobaObject* { exit(obj->val); }
    ));

    PostAssignmentSetup(collector);
}

std::string SystemObject::Representation() {
    return "system";
}
