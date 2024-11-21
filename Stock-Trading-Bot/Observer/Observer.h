#ifndef OBSERVER_H
#define OBSERVER_H

class Observer {
public:
    virtual void update() = 0;  // Pure virtual method to be implemented by observers
};

#endif
