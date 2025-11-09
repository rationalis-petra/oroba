#ifndef __OROBA_EVAL_HPP
#define __OROBA_EVAL_HPP

#include "gc/collector.hpp"
#include "oroba/data/bytecode.hpp"

OrobaObject* eval(Bytecode code, OrobaObject* activation, LocalCollector& collector);

#endif
