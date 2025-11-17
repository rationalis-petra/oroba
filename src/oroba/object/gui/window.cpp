#include "oroba/object/gui/window.hpp"

#include "oroba/data/error.hpp"
#include "oroba/object/primitives.hpp"

OrobaWindow::OrobaWindow() {
    set_title("Basic application");
    set_default_size(200, 200);
} 

OrobaObject* OrobaWindow::SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "present") {
        present();
        return NilObject::nil;
    } else if (name == "child:") {
        if (args.size() != 1)
            throw OrobaError("incorrect number of args to method child: in oroba-window");
        Gtk::Widget* child = dynamic_cast<Gtk::Widget*>(args[0]);
        if (!child)
            throw OrobaError("expected widget argument to method child: in oroba window");

        set_child(*child);
        return this;
    } else {
        return OrobaObject::SendMessage(internal, name, args, collector);
    }
}

std::string OrobaWindow::Representation() {
    return "window";
}
