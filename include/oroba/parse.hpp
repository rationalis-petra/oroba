#ifndef __OROBA_PARSE_HPP
#define __OROBA_PARSE_HPP

#include <istream>
#include <variant>

#include "gc/collector.hpp"
#include "oroba/data/bytecode.hpp"
#include "oroba/data/error.hpp"

variant<Bytecode, OrobaError> parse(istream& in, LocalCollector& collector);

#endif
