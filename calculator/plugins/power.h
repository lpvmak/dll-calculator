#ifndef POWER_H
#define POWER_H

#include "../calculator/plugin_base.h"

class Power : public Operation {
public:
    Power();
    virtual Operation *getOperation();
    virtual double execute(double left, double right);
};


#endif // !POWER_H
