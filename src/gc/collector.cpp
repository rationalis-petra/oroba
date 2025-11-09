#include "gc/collector.hpp"


Collectable::~Collectable() { }

unsigned long LocalCollector::CollectionThreashold = 16000;

LocalCollector::LocalCollector() { }

LocalCollector::~LocalCollector() {
    for (Collectable* c : values) {
        delete c;
    }
}

void LocalCollector::Add(Collectable* c) {
    values.push_back(c);
    c->tagged = false;
    AddedSinceLastCollection++;
}

bool LocalCollector::ShouldCollect() {
    return AddedSinceLastCollection > CollectionThreashold;
}

void LocalCollector::Collect() {
    AddedSinceLastCollection = 0;
    std::vector<Collectable*> new_values;
    for (Collectable* c : values) {
        if (!c->tagged) {
            delete c;
        } else {
            c->tagged = false;
            new_values.push_back(c);
        }
    }
    values = new_values;
}

void LocalCollector::TraceRoots() {
    for (auto tracer : roots) tracer->Trace();
    for (auto tracer : temp_roots) tracer->Trace();
    temp_roots.clear();
}

void LocalCollector::AddRoot(Tracer* tracer) {
    roots.insert(tracer);
}

void LocalCollector::RemoveRoot(Tracer* tracer) {
    roots.erase(tracer);
}
    
void LocalCollector::AddTempRoot(Tracer* tracer) {
    temp_roots.push_back(tracer);
}
    
