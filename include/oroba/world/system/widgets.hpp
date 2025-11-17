#ifndef __OROBA_WORLD_SYSTEM_WIDGETS_HPP
#define __OROBA_WORLD_SYSTEM_WIDGETS_HPP

#include <gtkmm/application.h>

#include "oroba/object/namespace.hpp"

class WidgetsNs : public NamespaceObject {
public:
    WidgetsNs(LocalCollector& collector);
    virtual std::string Representation() override;
};

#endif
