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
class PrimitiveMethodActivation : public OrobaObject {
public:
    PrimitiveMethodActivation(std::function<OrobaObject*(Args... args, LocalCollector& collector)> method, std::tuple<Args...> args)
        : m_method(method)
        , m_args(args) {}

    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override {
        throw InternalError("not-implemented: receiving messages as a primitive method");
    };

    virtual OrobaObject* Evaluate(LocalCollector& collector) override {
        return std::apply(m_method, std::tuple_cat(m_args, std::tuple<LocalCollector&>(collector)));
    };

private:
    std::tuple<Args...> m_args;
    std::function<OrobaObject*(Args... args, LocalCollector& collector)> m_method;
};

template <class... Args>
class PrimitiveMethod : public MethodObject {
public:
    PrimitiveMethod(std::string name, std::function<OrobaObject*(Args... args, LocalCollector& collector)> method) : m_method(method) {};
    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override {
        throw InternalError("not-implemented: receiving messages as a primitive method");
    }

    virtual OrobaObject* MakeActivationObject(std::vector<OrobaObject*> args, LocalCollector& collector) override {
        if (args.size() == sizeof...(Args)) {
            PrimitiveMethodActivation<Args...>* activation = new PrimitiveMethodActivation<Args...>(m_method, convert_to_tuple<0, Args...>(args));
            collector.Add(activation);
            return activation;
        } else {
            std::ostringstream oss;
            oss << "incorrect number of args - expected " << sizeof...(Args) << " but have " << args.size();
            throw InternalError(oss.str());
        }
    }

private:
    std::string m_name;
    std::function<OrobaObject*(Args... args, LocalCollector& collector)> m_method;
};


#endif
