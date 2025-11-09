#ifndef __OROBA_DATA_ERROR_HPP
#define __OROBA_DATA_ERROR_HPP

#include <ostream>
#include <string>

typedef unsigned long inputpos;

struct OrobaError {
    std::string message;
    inputpos start;
    inputpos end;
};

std::ostream& operator<<(std::ostream& out, OrobaError error);

#endif
