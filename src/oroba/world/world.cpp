#include "oroba/world/world.hpp"

#include "oroba/object/primitives.hpp"
#include "oroba/world/system.hpp"
#include "oroba/world/math.hpp"
#include "oroba/world/os.hpp"

#include "oroba/world/lobby.hpp"

using namespace std;

WorldObject::WorldObject(LocalCollector& collector) {
    AddValue("true", new BooleanObject(true));
    AddValue("false", new BooleanObject(false));

    AddValue("os", new OSObject(collector));
    AddValue("system", new SystemObject(collector));
    AddValue("math", new MathObject(collector));

    NilObject::nil = new NilObject();
    AddValue("nil", NilObject::nil);

    AddValue("lobby", new LobbyObject(slots["math"].object, this, collector));

    PostAssignmentSetup(collector);
}

std::string WorldObject::Representation() {
    return "world";
}
