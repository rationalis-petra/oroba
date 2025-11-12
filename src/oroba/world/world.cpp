#include "oroba/world/world.hpp"

#include "oroba/object/primitives.hpp"
#include "oroba/world/system.hpp"
#include "oroba/world/os.hpp"

using namespace std;

WorldObject::WorldObject(LocalCollector& collector) {
    AddValue("true", new BooleanObject(true));
    AddValue("false", new BooleanObject(false));

    AddValue("os", new OSObject(collector));
    AddValue("system", new SystemObject(collector));

    NilObject::nil = new NilObject();
    AddValue("nil", NilObject::nil);

    PostAssignmentSetup(collector);
}
