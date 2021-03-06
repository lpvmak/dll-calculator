#ifndef TRIGONOMETRY_H
#define TRIGONOMETRY_H

#include <iostream>
#include <string> 
#include "expression.h"
#include "operation.h"

class Function : public Expression {   
public:
    ExpressionInterface* expression;

    Function(const std::string &str, Operation  *fun);
    ~Function();

    double calculate();
};

class Constant : public Expression {
public:
    Constant(Operation *constant);

    double calculate();
};

#endif //TRIGONOMETRY_H