#include "subtraction.h"

Subtraction::Subtraction() : Operation(OperationType::BINARY, "-", 1) {}
double Subtraction::execute(double left, double right) {
    double num = (left - right);
    return num;
}
Operation* Subtraction::getOperation() {
    return new Subtraction();
}

Operation* loadOperation() {
    return new Subtraction();
}