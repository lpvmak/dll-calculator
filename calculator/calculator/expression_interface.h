#ifndef EXPRESSION_INTERFACE_H
#define EXPRESSION_INTERFACE_H

class Expression;


class Operation;


class ExpressionInterface {
public:
    bool isNegative;
    bool isUnary;
    Operation *base_operations;

    //TO DO: safe handling of adding expressions to children
    virtual Expression *getExpression() { return 0; }
    virtual double calculate() { return 0; }
    virtual ~ExpressionInterface() { delete base_operations; }
};

#endif //EXPRESSION_INTERFACE_H