#include <gtkmm/window.h>

#include "oroba/object/object.hpp"

class OrobaWindow : public OrobaObject, public Gtk::Window {
public:
    OrobaWindow();

    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override;
    virtual std::string Representation() override;
};

