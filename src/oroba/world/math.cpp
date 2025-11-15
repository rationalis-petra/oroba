#include "oroba/world/math.hpp"

#include <iostream>

#include "oroba/object/primitives.hpp"
#include "oroba/object/multimethod.hpp"

using namespace std;

MathObject::MathObject(LocalCollector& collector) {
    AddValue("add", new PrimFunction<IntegerObject*, IntegerObject*>(
      "add",
      [](IntegerObject* lhs, IntegerObject* rhs, LocalCollector& collector) -> OrobaObject* {
          IntegerObject* out = new IntegerObject(lhs->val + rhs->val);
          collector.Add(out);
          return out;
      })
    );

    PostAssignmentSetup(collector);
}
