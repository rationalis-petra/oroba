#ifndef __OROBA_OBJECT_MULTIMETHOD_HPP
#define __OROBA_OBJECT_MULTIMETHOD_HPP

#include "oroba/object/primitive_method.hpp"

// Multimethods:
// respond to messages:
// - 'add-method'
// - e.g.
// (+) add-method: {|lhs <- traits point. rhs <- traits point.|
//   point clone x: (lhs x) + (rhs x); y: (lhs y)  + (rhs y).
// }


// Simple Methods/Functions
// - 'add-method'
// - e.g.
// (+) with: 1, 2.

template <class... Args>
class PrimFunction : public OrobaObject {
public:
    PrimFunction(std::string name, std::function<OrobaObject*(Args... args, LocalCollector& collector)> method) : m_method(method) {};

    virtual OrobaObject* SendMessage(bool internal, std::string name, std::vector<OrobaObject*> args, LocalCollector& collector) override {
        if (name == "_invoke:") {
            if (args.size() == sizeof...(Args)) {
                auto args_tuple = convert_to_tuple<0, Args...>(args);
                return std::apply(m_method, std::tuple_cat(args_tuple, std::tuple<LocalCollector&>(collector)));
            } else {
                std::ostringstream oss;
                oss << "incorrect number of args - expected " << sizeof...(Args) << " but have " << args.size();
                throw InternalError(oss.str());
            }
        } else {
            return OrobaObject::SendMessage(internal, name, args, collector);
        }
    }

    virtual std::string Representation() override {
        return m_name;
    }

private:
    std::string m_name;
    std::function<OrobaObject*(Args... args, LocalCollector& collector)> m_method;
};

#endif
