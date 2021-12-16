#ifndef E_H
#define E_H

#include "../calculator/plugin_base.h"

class E : public Operation {
public:
    E();
    virtual Operation *getOperation();
    virtual double execute(double left, double right);
};

#endif // !E_H

