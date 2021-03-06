#ifndef MULTIPLY_H
#define MULTIPLY_H

#include "../calculator/plugin_base.h"

class Multiply : public Operation {
public:
    inline Multiply();

    inline virtual Operation *getOperation();

    double execute(double left, double right);
};

#endif // !MULTIPLY_H

