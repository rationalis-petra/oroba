#ifndef __OROBA_OBJECT_OSTREAM_HPP
#define __OROBA_OBJECT_OSTREAM_HPP

#include <ostream>
#include "oroba/object/object.hpp"

class OStreamObject : public OrobaObject {
public:
    OStreamObject(std::ostream& ostream);
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override;

    std::ostream& ostream;
};

#endif
