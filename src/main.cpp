#include <iostream>
#include <fstream>
#include <variant>
#include <string>

#include "oroba/data/error.hpp"

#include "oroba/parse/parse.hpp"
#include "oroba/eval.hpp"
#include "oroba/object/collections.hpp"
#include "oroba/world/world.hpp"

#include "command_line_args.hpp"

using namespace std;

const string version = "0.0.0";

bool repl_iter(istream& in, ostream& out, OrobaObject* active, LocalCollector& collector) {
    cout << "> ";

    variant<Bytecode, ParseError> parsed = parse(in, collector);
    if (holds_alternative<Bytecode>(parsed)) {
        Bytecode code = get<Bytecode>(parsed);
        try {
            OrobaObject* result = eval(code, active, collector); 
            OrobaObject* result_tostr = result->SendMessage(false, "to-string", vector<OrobaObject*>(), collector);
            StringObject* str = dynamic_cast<StringObject*>(result_tostr);
            if (str) {
                out << str->data;
            } else {
                out << "object to-string method did not return a string!";
            }
        } catch (OrobaError err) {
            out << err.message;
        }

        out << "\n";
        out.flush();
        return true;
    } else {
        out << get<ParseError>(parsed);
        out << "\n";
        out.flush();
        return false;
    }
}

bool file_iter(istream& in, ostream& out, OrobaObject* active, LocalCollector& collector) {
    variant<Bytecode, ParseError> parsed = parse(in, collector);
    if (holds_alternative<Bytecode>(parsed)) {
        Bytecode code = get<Bytecode>(parsed);
        try {
            eval(code, active, collector); 
        } catch (OrobaError err) {
            out << err.message;
        }
        return true;
    } else {
        out << get<ParseError>(parsed);
        out << "\n";
        out.flush();
        return false;
    }
}

int main (int argc, char** argv) {

    LocalCollector main_collector;
    WorldObject* world = new WorldObject(main_collector);
    main_collector.AddRoot(world);
    main_collector.Add(world);

    CommandLineArgs args = parse_args(argc, argv);

    if (holds_alternative<ReplCommand>(args)) {
        cout << "Oroba\n";
        cout << "  version: " << version << "\n";
        try {
            while (repl_iter(cin, cout, world->slots["lobby"].object, main_collector));
        } catch (InternalError& err) {
            cout << "Threw error indication exceptional circumstance:\n" << err.message << "\n";
            cout.flush();
        }
    } else if (holds_alternative<RunCommand>(args)) {
        RunCommand cmd = get<RunCommand>(args);
        try {
            std::ifstream is(cmd.filename);
            while (file_iter(is, cout, world->slots["lobby"].object, main_collector));
        } catch (InternalError& err) {
            cout << "Threw error indication exceptional circumstance:\n" << err.message << "\n";
            cout.flush();
        }
    } else {
        CLIError err = get<CLIError>(args);
        cout << "command line arg parsing failed with message:\n  " << err.message;
    }
    return 0;
}
