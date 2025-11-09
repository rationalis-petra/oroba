#include "oroba/data/error.hpp"

OrobaError::OrobaError(std::string _message) : message(_message) {}

InternalError::InternalError(std::string _message) : message(_message) {}

std::ostream& operator<<(std::ostream& out, ParseError error) {
    out << error.message;
    return out;
}
