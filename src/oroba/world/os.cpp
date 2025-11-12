#include "oroba/world/os.hpp"

#include <iostream>

#include "oroba/object/ostream.hpp"

using namespace std;

OSObject::OSObject(LocalCollector& collector) {
    AddValue("stdout", new OStreamObject(cout));

    PostAssignmentSetup(collector);
}
