#include "oroba/kernel/traits.hpp"

#include <iostream>

#include "oroba/objects/numeric.hpp"
#include "oroba/objects/composite.hpp"
#include "oroba/objects/primitive_method.hpp"

using namespace std;

OrobaObject* mk_scope_trait(LocalCollector& collector) {
    CompositeObject* scope = new CompositeObject;
   //  scope->AddMethod("def:as:", new TypedPrimitiveMethod<OrobaObjet*, SymbolObect*>(
   //    "def:as:",
   //    [](OrobaObjet* dest, SymbolObject* sym, LocalCollector& collector) -> OrobaObject* {
   //        dest->
   //        return dest;
   //    }
   // ))

    // scope->AddMethod("let:val:", new TypedPrimitiveMethod<CompositeObject*, SymbolObject*, OrobaObject*>(
    //   "let:val:",
    //   [](CompositeObject* dest, SymbolObject* sym, OrobaObject* val, LocalCollector& collector) -> OrobaObject* {
    //       string name = sym->symbol;
    //       Slot s;
    //       s.parent_priority = 0;
    //       s.object = val;
    //       dest->slots[name] = s;

    //       SlotMethod m{false, name};
    //       dest->methods[name] = Method(m);
          
    //       return dest;
    // }));

    // scope->AddMethod("var:val:", new TypedPrimitiveMethod<CompositeObject*, SymbolObject*, OrobaObject*>(
    //   "var:val:",
    //   [](CompositeObject* dest, SymbolObject* sym, OrobaObject* val, LocalCollector& collector) -> OrobaObject* {
    //       string name = sym->symbol;
    //       Slot s;
    //       s.parent_priority = 0;
    //       s.object = val;
    //       dest->slots[name] = s;

    //       SlotMethod m{false, name};
    //       dest->methods[name] = Method(m);

    //       string settername = name + ":";
    //       m = SlotMethod{true, name};
    //       dest->methods[settername] = Method(m);
          
    //       return dest;
    // }));

    return scope;
}

OrobaObject* mk_bool_trait(LocalCollector& collector) {
    // ifTrue:ifFalse: method
    CompositeObject* bool_trait = new CompositeObject;

    // cmp->AddMethod("ifTrue:ifFalse:", new TypedPrimitiveMethod<OrobaObejct*, OrobaObject*>(
    //   "exit:",
    //   [](IntegerObject* obj, LocalCollector& collector) -> OrobaObject* { exit(obj->val); }
    // ));
    return bool_trait;
}

OrobaObject* TraitsNs::bool_trait = nullptr; 
OrobaObject* TraitsNs::scope_trait = nullptr; 

TraitsNs::TraitsNs(LocalCollector& collector) {
    bool_trait = mk_bool_trait(collector);
    AddValue("boolean", bool_trait);

    scope_trait = mk_scope_trait(collector);
    AddValue("scope", scope_trait);

    PostAssignmentSetup(collector);
}

std::string TraitsNs::Representation() {
    return "traits";
}
