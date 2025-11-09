#ifndef __COLLECTOR_HPP
#define __COLLECTOR_HPP

#include <vector>

struct Collectable {
public:
    virtual ~Collectable();
    bool tagged;
};

class LocalCollector {
public:
    LocalCollector();
    ~LocalCollector();

    void Add(Collectable* c);

    bool ShouldCollect();
    void Collect();
    
private:
    static unsigned long CollectionThreashold;
    std::vector<Collectable*> values;
};


#endif

