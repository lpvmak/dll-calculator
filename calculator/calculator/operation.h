#ifndef OPERATION_H
#define OPERATION_H

#include <math.h>
#include <iostream>
#include <string> 
#include <regex>
#include "expression_interface.h"

enum class OperationType {
    CONSTANT,
    BINARY,
    FUNCTION,
} ;

class Operation {
protected:
    OperationType _type;
    std::string _name;
    int _prior;
public:
    Operation(OperationType const & operationType, std::string const & operationName, int prior=0);
    OperationType getType();
    std::string getName();
    int getPrior();
    virtual Operation* getOperation() = 0;
    virtual double execute(double left, double right) = 0;
};

class BaseLexems {
public:
    static char unary_minus;
    static char unary_plus;
    static char rbracket;
    static char lbracket;

};

#endif //OPERATION_H