#ifndef __OROBA_DATA_ERROR_HPP
#define __OROBA_DATA_ERROR_HPP

#include <ostream>
#include <string>

class OrobaObject;

typedef unsigned long inputpos;

struct ParseError {
    std::string message;
    inputpos start;
    inputpos end;
};

struct InternalError {
    InternalError(std::string message);
    std::string message;
};

struct OrobaError {
    OrobaError(std::string message);
    std::string message;
};

std::ostream& operator<<(std::ostream& out, ParseError error);

#endif
