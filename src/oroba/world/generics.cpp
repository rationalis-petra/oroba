#include "oroba/world/generics.hpp"

#include <iostream>

#include "oroba/object/primitives.hpp"
#include "oroba/object/multimethod.hpp"

using namespace std;

GenericsNs::GenericsNs(LocalCollector& collector) {
}

std::string GenericsNs::Representation() {
    return "world generics";
}
