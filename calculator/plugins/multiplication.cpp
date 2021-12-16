#include "multiplication.h"

Multiply::Multiply() : Operation(OperationType::BINARY, "*", 2) { }

Operation* Multiply::getOperation() {
    return new Multiply();
}

double Multiply::execute(double left, double right) {
    double num = (left*  right);
    return num;
}

Operation* loadOperation() {
    return new Multiply();
}