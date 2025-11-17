#include "oroba/world/system.hpp"

#include <iostream>

#include "oroba/object/primitives.hpp"
#include "oroba/object/primitive_method.hpp"
#include "oroba/object/ostream.hpp"
#include "oroba/object/gui/window.hpp"

#include "oroba/world/system/widgets.hpp"

using namespace std;

SystemNs::SystemNs(LocalCollector& collector)
    : app(Gtk::Application::create("org.oroba")) {
    AddValue("widgets", new WidgetsNs(collector));

    AddValue("stdout", new OStreamObject(cout));

    AddMethod("exit", Method(new TypedPrimitiveMethod<OrobaObject*>(
      "exit",
      [](OrobaObject* self, LocalCollector& collector) -> OrobaObject* { exit(0); })
    ));

    AddMethod("exit:", new TypedPrimitiveMethod<OrobaObject*, IntegerObject*>(
      "exit:",
      [](OrobaObject* self, IntegerObject* obj, LocalCollector& collector) -> OrobaObject* {
          exit(obj->val);
      }
    ));

    AddMethod("start-winmanager:", new TypedPrimitiveMethod<OrobaObject*, OrobaObject*>(
      "start-winmanager:",
      [this](OrobaObject* self, OrobaObject* callback, LocalCollector& collector) -> OrobaObject* {
          // TODO: add callback as root?
          app->signal_activate().connect([callback, &collector]() {
              callback->SendMessage(false, "invoke", vector<OrobaObject*>(), collector);
          });
          app->run(0, NULL);
          return NilObject::nil;
      }
    ));

    AddMethod("create-window", new TypedPrimitiveMethod<OrobaObject*>(
      "create-window",
      [this](OrobaObject* self, LocalCollector& collector) -> OrobaObject* {
          OrobaWindow* win = new OrobaWindow;
          collector.Add(win);
          app->add_window(*win);
          return win;
      }
    ));

    PostAssignmentSetup(collector);
}

std::string SystemNs::Representation() {
    return "system";
}
