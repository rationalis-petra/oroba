#include "command_line_args.hpp"

#include <vector>

using namespace std;

CommandLineArgs parse_run(vector<string> args) {
    if (args.size() == 1) {
        RunCommand run;
        run.filename = args[0];
        return run;
    } else {
        CLIError err;
        err.message = "run expecting a filename, got none.";
        return err;
    }
}

CommandLineArgs parse_args(int argc, char** argv) {
    if (argc > 1) {
        string subcommand = string(argv[1]);
        if (subcommand == "run") {
            vector<string> args;
            for (int i = 2; i < argc; i++) {
                args.push_back(string(argv[i]));
            }
            return parse_run(args);
        } else {
            CLIError err;
            err.message = "unrecognized subcommand: " + subcommand;
            return err;
        }
    } else {
        return ReplCommand();
    }
}
