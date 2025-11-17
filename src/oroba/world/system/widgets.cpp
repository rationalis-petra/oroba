#include "oroba/world/system/widgets.hpp"

#include "oroba/object/primitives.hpp"
#include "oroba/object/primitive_method.hpp"
#include "oroba/object/ostream.hpp"

#include "oroba/object/gui/textview.hpp"

using namespace std;

WidgetsNs::WidgetsNs(LocalCollector& collector) {
    AddMethod("text-view", new TypedPrimitiveMethod<OrobaObject*>(
      "text-view",
      [this](OrobaObject* self, LocalCollector& collector) -> OrobaObject* {
          OrobaTextView* view = new OrobaTextView;
          collector.Add(view);
          return view;
      }
    ));
    PostAssignmentSetup(collector);
}

std::string WidgetsNs::Representation() {
    return "*system widgets*";
}
