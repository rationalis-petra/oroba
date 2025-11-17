#include <gtkmm/textview.h>

#include "oroba/object/object.hpp"

class OrobaTextView : public OrobaObject, public Gtk::TextView {
public:
    OrobaTextView();

    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override;
    virtual std::string Representation() override;
};

