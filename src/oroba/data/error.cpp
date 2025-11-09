#include "oroba/data/error.hpp"

std::ostream& operator<<(std::ostream& out, OrobaError error) {
    out << error.message;
    return out;
}
