#ifndef __OROBA_WORLD_KERNEL_HPP
#define __OROBA_WORLD_KERNEL_HPP

#include "oroba/objects/namespace.hpp"

class KernelNs : public NamespaceObject {
public:
    KernelNs(LocalCollector& collector);
    virtual std::string Representation() override;
};

#endif
