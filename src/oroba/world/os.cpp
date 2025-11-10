#include "oroba/world/os.hpp"

#include <iostream>

#include "oroba/object/ostream.hpp"

using namespace std;

OSObject::OSObject(LocalCollector& collector) {
    slots["stdout"] = new OStreamObject(cout);

    for (auto& pr : slots) {
        collector.Add(pr.second);
    }
}
