#include "oroba/world/traits.hpp"

#include <iostream>

#include "oroba/object/primitives.hpp"
#include "oroba/object/primitive_method.hpp"

using namespace std;

OrobaObject* block_trait(LocalCollector& collector) {
    return nullptr;
}

TraitsObject::TraitsObject(LocalCollector& collector) {
    //methods["block"] = block_trait(collector);

}
