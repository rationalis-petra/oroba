#include "oroba/world/ports.hpp"

#include <iostream>

#include "oroba/object/ostream.hpp"

using namespace std;

PortsObject::PortsObject(LocalCollector& collector) {
    children["stdout"] = new OStreamObject(cout);

    for (auto& pr : children) {
        collector.Add(pr.second);
    }
}
