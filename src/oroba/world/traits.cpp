#include "oroba/world/traits.hpp"

#include <iostream>

#include "oroba/object/primitives.hpp"
#include "oroba/object/composite.hpp"
#include "oroba/object/primitive_method.hpp"

using namespace std;

OrobaObject* mk_block_trait(LocalCollector& collector) {
    return nullptr;
}

OrobaObject* mk_bool_trait(LocalCollector& collector) {
    // ifTrue:ifFalse: method
    CompositeObject* cmp = new CompositeObject;

    // cmp->AddMethod("ifTrue:ifFalse:", new TypedPrimitiveMethod<OrobaObejct*, OrobaObject*>(
    //   "exit:",
    //   [](IntegerObject* obj, LocalCollector& collector) -> OrobaObject* { exit(obj->val); }
    // ));
    return cmp;
}

OrobaObject* TraitsNs::bool_trait = nullptr; 
TraitsNs::TraitsNs(LocalCollector& collector) {
    bool_trait = mk_bool_trait(collector);
    AddValue("boolean", bool_trait);

    PostAssignmentSetup(collector);
}

std::string TraitsNs::Representation() {
    return "traits";
}
