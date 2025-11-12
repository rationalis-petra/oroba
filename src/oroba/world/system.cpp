#include "oroba/world/system.hpp"

#include <iostream>

#include "oroba/object/primitives.hpp"
#include "oroba/object/primitive_method.hpp"

using namespace std;

SystemObject::SystemObject(LocalCollector& collector) {
    /*
    AddMethod("exit", )
    methods["exit"] = new PrimitiveMethod<>(
      "exit",
      [](LocalCollector& collector) -> OrobaObject* { exit(0); }
    );
    methods["exit:"] = new PrimitiveMethod<IntegerObject*>(
      "exit:",
      [](IntegerObject* obj, LocalCollector& collector) -> OrobaObject* { exit(obj->val); }
    );
    */

    for (auto& pr : slots) {
        collector.Add(pr.second.object);
    }
}
