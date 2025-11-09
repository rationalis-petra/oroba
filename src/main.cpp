#include <iostream>
#include <variant>
#include <string>

#include "oroba/data/error.hpp"

#include "oroba/parse.hpp"
#include "oroba/eval.hpp"
#include "oroba/object/collections.hpp"
#include "oroba/world/world.hpp"

using namespace std;

const string version = "0.0.0";

bool repl_iter(istream& in, ostream& out, OrobaObject* active, LocalCollector& collector) {
    cout << "> ";

    variant<Bytecode, OrobaError> parsed = parse(in, collector);
    if (holds_alternative<Bytecode>(parsed)) {
        Bytecode code = get<Bytecode>(parsed);
        OrobaObject* result = eval(code, active, collector); 
        OrobaObject* result_tostr = result->SendExternalMessage("to-string", vector<OrobaObject*>(), collector);
        StringObject* str = dynamic_cast<StringObject*>(result_tostr);

        if (str) {
            out << str->data;
        } else {
            out << "object to-string method did not return a string!";
        }
        out << "\n";
        out.flush();
        return true;
    } else {
        out << get<OrobaError>(parsed);
        out << "\n";
        out.flush();
        return false;
    }
}

int main (int argc, char** argv) {
    cout << "Oroba\n";
    cout << "  version: " << version << "\n";

    LocalCollector main_collector;
    WorldObject* world = new WorldObject(main_collector);
    main_collector.AddRoot(world);
    main_collector.Add(world);
    try {
        while (repl_iter(cin, cout, world, main_collector));
    } catch (std::string& err) {
        cout << "Threw error indication exceptional circumstance:\n" << err << "\n";
        cout.flush();
    }
    return 0;
}
