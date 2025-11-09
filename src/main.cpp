#include <iostream>
#include <variant>
#include <string>

#include "oroba/data/error.hpp"

#include "oroba/parse.hpp"
#include "oroba/eval.hpp"

#include "oroba/object/collections.hpp"

using namespace std;

const string version = "0.0.0";

bool repl_iter(istream& in, ostream& out, LocalCollector& collector) {
    cout << "> ";

    variant<Bytecode, OrobaError> parsed = parse(in, collector);
    if (holds_alternative<Bytecode>(parsed)) {
        Bytecode code = get<Bytecode>(parsed);
        OrobaObject* result = eval(code, nullptr, collector); 
        OrobaObject* result_tostr = result->SendExternalMessage("to-string", vector<OrobaObject*>());
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
    while (repl_iter(cin, cout, main_collector));
    return 0;
}
