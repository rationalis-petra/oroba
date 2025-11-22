#include "oroba/kernel/math.hpp"

#include <iostream>

#include "oroba/objects/numeric.hpp"
#include "oroba/objects/multimethod.hpp"

using namespace std;

MathObject::MathObject(LocalCollector& collector) {
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

std::string MathObject::Representation() {
    return "kernel math";
}
