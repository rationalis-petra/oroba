#ifndef __COLLECTOR_HPP
#define __COLLECTOR_HPP

#include <vector>
#include <unordered_set>

struct Collectable {
public:
    virtual ~Collectable();
    bool tagged = 0;
};

struct Tracer {
public:
    virtual void Trace() = 0;
};

class LocalCollector {
public:
    LocalCollector();
    ~LocalCollector();

    void Add(Collectable* c);

    bool ShouldCollect();
    void Collect();

    void AddRoot(Tracer* tracer);
    void RemoveRoot(Tracer* tracer);

    void AddTempRoot(Tracer* tracer);
    void TraceRoots();
    
private:
    static unsigned long CollectionThreashold;
    unsigned long AddedSinceLastCollection = 0;
    std::vector<Collectable*> values;

    std::unordered_set<Tracer*> roots;
    std::vector<Tracer*> temp_roots;
};


#endif

