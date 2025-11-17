#include "oroba/object/gui/textview.hpp"

#include "oroba/object/primitives.hpp"

OrobaTextView::OrobaTextView() {
} 

OrobaObject* OrobaTextView::SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) {
    return OrobaObject::SendMessage(internal, name, args, collector);
}

std::string OrobaTextView::Representation() {
    return "text-view";
}
