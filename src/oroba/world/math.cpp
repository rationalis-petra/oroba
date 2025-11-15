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

    AddValue("sub", new PrimFunction<IntegerObject*, IntegerObject*>(
      "sub",
      [](IntegerObject* lhs, IntegerObject* rhs, LocalCollector& collector) -> OrobaObject* {
          IntegerObject* out = new IntegerObject(lhs->val - rhs->val);
          collector.Add(out);
          return out;
      })
    );

    AddValue("mul", new PrimFunction<IntegerObject*, IntegerObject*>(
      "mul",
      [](IntegerObject* lhs, IntegerObject* rhs, LocalCollector& collector) -> OrobaObject* {
          IntegerObject* out = new IntegerObject(lhs->val * rhs->val);
          collector.Add(out);
          return out;
      })
    );

    AddValue("div", new PrimFunction<IntegerObject*, IntegerObject*>(
      "div",
      [](IntegerObject* lhs, IntegerObject* rhs, LocalCollector& collector) -> OrobaObject* {
          IntegerObject* out = new IntegerObject(lhs->val / rhs->val);
          collector.Add(out);
          return out;
      })
    );

    AddValue("equal?", new PrimFunction<IntegerObject*, IntegerObject*>(
      "equal?",
      [](IntegerObject* lhs, IntegerObject* rhs, LocalCollector& collector) -> OrobaObject* {
          BooleanObject* out = new BooleanObject(lhs->val == rhs->val);
          collector.Add(out);
          return out;
      })
    );

    AddValue("less-or-equal?", new PrimFunction<IntegerObject*, IntegerObject*>(
      "less-or-equal?",
      [](IntegerObject* lhs, IntegerObject* rhs, LocalCollector& collector) -> OrobaObject* {
          BooleanObject* out = new BooleanObject(lhs->val <= rhs->val);
          collector.Add(out);
          return out;
      })
    );

    AddValue("greater?", new PrimFunction<IntegerObject*, IntegerObject*>(
      "greater?",
      [](IntegerObject* lhs, IntegerObject* rhs, LocalCollector& collector) -> OrobaObject* {
          BooleanObject* out = new BooleanObject(lhs->val > rhs->val);
          collector.Add(out);
          return out;
      })
    );

    AddValue("greater-or-equal?", new PrimFunction<IntegerObject*, IntegerObject*>(
      "greater-or-equal?",
      [](IntegerObject* lhs, IntegerObject* rhs, LocalCollector& collector) -> OrobaObject* {
          BooleanObject* out = new BooleanObject(lhs->val >= rhs->val);
          collector.Add(out);
          return out;
      })
    );

    PostAssignmentSetup(collector);
}
