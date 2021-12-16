#include "division.h"
Division::Division() : Operation(OperationType::BINARY, "/", 2) {}

double Division::execute(double left, double right) {
    double num = (left / right);
    return num;
}
Operation* Division::getOperation() {
    return new Division();
}

Operation *loadOperation() {
    return new Division();
}