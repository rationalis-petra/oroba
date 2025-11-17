#include "oroba/object/gui/window.hpp"

#include "oroba/object/primitives.hpp"

OrobaWindow::OrobaWindow() {
    set_title("Basic application");
    set_default_size(200, 200);
} 

OrobaObject* OrobaWindow::SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    if (name == "present") {
        present();
        return NilObject::nil;
    } else {
        return OrobaObject::SendMessage(internal, name, args, collector);
    }
}

std::string OrobaWindow::Representation() {
    return "window";
}
