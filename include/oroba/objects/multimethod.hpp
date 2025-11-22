#ifndef __OROBA_OBJECT_MULTIMETHOD_HPP
#define __OROBA_OBJECT_MULTIMETHOD_HPP

#include <unordered_map>

#include "oroba/objects/primitive.hpp"
#include "oroba/objects/primitive_method.hpp"

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
class PrimFunction : public PrimitiveObject {
public:
    PrimFunction(std::string name, std::function<OrobaObject*(Args... args, LocalCollector& collector)> method)
        : m_method("_invoke", method)
        , m_name(name) {};

    virtual std::unordered_map<std::string, PrimitiveMethod*> GetPrimitives() override {
        std::unordered_map<std::string, PrimitiveMethod*> out; 
        out["invoke"] = &m_method;
        return out;
    }

    virtual std::string Representation() override {
        return m_name;
    }

private:
    std::string m_name;
    TypedPrimitiveMethod<Args...> m_method;
    //std::function<OrobaObject*(Args... args, LocalCollector& collector)> m_method;
};

#endif
