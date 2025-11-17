#ifndef __COMMAND_LINE_ARGS_HPP
#define __COMMAND_LINE_ARGS_HPP

#include <optional>
#include <variant>
#include <string>

struct RunCommand {
    std::string filename;
};

struct ReplCommand {
};

struct CLIError {
    std::string message;
};

typedef std::variant<ReplCommand, RunCommand, CLIError> CommandLineArgs;

CommandLineArgs parse_args(int argc, char** argv); 

#endif
