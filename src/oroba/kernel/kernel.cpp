#include "oroba/kernel/kernel.hpp"
#include "oroba/kernel/world.hpp"
#include "oroba/kernel/traits.hpp"
#include "oroba/kernel/system.hpp"
#include "oroba/kernel/math.hpp"

#include "oroba/objects/numeric.hpp"

using namespace std;

KernelNs::KernelNs(LocalCollector& collector) {
    AddValue("traits", new TraitsNs(collector));

    AddValue("true", new BooleanObject(true));
    AddValue("false", new BooleanObject(false));

    AddValue("system", new SystemNs(collector));
    AddValue("math", new MathObject(collector));

    NilObject::nil = new NilObject();
    AddValue("nil", NilObject::nil);

    PostAssignmentSetup(collector);
}

std::string KernelNs::Representation() {
    return "kernel";
}
