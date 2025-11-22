#include "oroba/objects/init.hpp"

#include "oroba/objects/system/ostream.hpp"
#include "oroba/objects/collections.hpp"
#include "oroba/objects/numeric.hpp"

void init_primitives() {
    OStreamObject::Init();
    NilObject::Init();
    BooleanObject::Init();
    IntegerObject::Init();
    StringObject::Init();
}

void teardown_primitives() {
    OStreamObject::Teardown();
    NilObject::Teardown();
    BooleanObject::Teardown();
    IntegerObject::Teardown();
    StringObject::Teardown();
}
