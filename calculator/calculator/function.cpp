#include "function.h"

Function::Function(const std::string &str, Operation  *fun) {
    base_operations = fun;
    int i = 0;
    while (isalpha(str[i])) i++;
    addExpression(substr(str, i, str.length() - i), this->expression);
}

Function::~Function() {
    delete expression;
}

double Function::calculate() {   
    double num = base_operations->execute(expression->calculate(), 0);
    return  (isNegative) ? num * (-1) : num;
}

Constant::Constant(  Operation * constant) {
    base_operations = constant;
}

double Constant::calculate() {
    double num = base_operations->execute(0, 0);
    return (isNegative) ? num * (-1) : num;
}
