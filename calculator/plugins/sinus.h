#ifndef SINUS_H
#define SINUS_H

#include "../calculator/plugin_base.h"

class Sinus : public Operation {
public:
    Sinus();
    virtual Operation *getOperation();
    virtual double execute(double left, double right);
};

#endif SINUS_H