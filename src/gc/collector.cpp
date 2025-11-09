#include "gc/collector.hpp"


Collectable::~Collectable() { }

LocalCollector::LocalCollector() { }

LocalCollector::~LocalCollector() {
    for (Collectable* c : values) {
        delete c;
    }
}

void LocalCollector::Add(Collectable* c) {
    values.push_back(c);
}

bool LocalCollector::ShouldCollect() {
    return true;
}

void LocalCollector::Collect() {
    for (Collectable* c : values) {
        if (!c->tagged) delete c;
    }
}
    
