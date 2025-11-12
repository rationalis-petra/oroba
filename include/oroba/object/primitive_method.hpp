#ifndef __OROBA_OBJECT_PRIMITIVE_METHOD_HPP
#define __OROBA_OBJECT_PRIMITIVE_METHOD_HPP

#include <functional>
#include <cstdint>
#include <string>
#include <sstream>

#include "oroba/data/error.hpp"
#include "oroba/object/object.hpp"

struct IncorrectNumberOfArgsError { uint16_t expected; uint16_t actual; };
struct InvalidArgTypeError {};

template <int idx> std::tuple<> convert_to_tuple(std::vector<OrobaObject*> args) {
    return std::tuple<>{};
}

template <int idx, class Arg, class... Args> std::tuple<Arg, Args...> convert_to_tuple(std::vector<OrobaObject*> args) {
    Arg arg = dynamic_cast<Arg>(args[idx]);
    if (arg == nullptr) {
        throw InvalidArgTypeError();
    }

    std::tuple<Args...> tup = convert_to_tuple<idx+1, Args...>(args);
    return std::tuple_cat(std::tuple<Arg>(arg), tup);
}

template <class... Args>
class TypedPrimitiveMethod : public PrimitiveMethod {
public:
    TypedPrimitiveMethod(std::string name, std::function<OrobaObject*(Args... args, LocalCollector& collector)> method) : m_method(method) {};

    virtual OrobaObject* Invoke(std::vector<OrobaObject*> args, LocalCollector& collector) override {
        if (args.size() == sizeof...(Args)) {
            auto args_tuple = convert_to_tuple<Args...>(args);
            return std::apply(m_method, std::tuple_cat(args_tuple, std::tuple<LocalCollector&>(collector)));
        } else {
            std::ostringstream oss;
            oss << "incorrect number of args - expected " << sizeof...(Args) << " but have " << args.size();
            throw InternalError(oss.str());
        }
    }

    virtual void Trace() override {}; 

private:
    std::string m_name;
    std::function<OrobaObject*(Args... args, LocalCollector& collector)> m_method;
};


#endif
