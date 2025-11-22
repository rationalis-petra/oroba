#include "oroba/kernel/system.hpp"

#include <iostream>

#include "oroba/objects/primitive_method.hpp"
#include "oroba/objects/numeric.hpp"
#include "oroba/objects/system/ostream.hpp"

using namespace std;

SystemNs::SystemNs(LocalCollector& collector) {
    AddValue("stdout", new OStreamObject(cout));

    PostAssignmentSetup(collector);
}

std::string SystemNs::Representation() {
    return "system";
}
