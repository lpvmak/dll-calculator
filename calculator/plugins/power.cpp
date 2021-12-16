#include "power.h"

Power::Power() : Operation(OperationType::BINARY, "^", 3) {}

double Power::execute(double left, double right) {
    double num = pow(left, right);
    return num;
}
Operation* Power::getOperation() {
    return new Power();
}

Operation* loadOperation() {
    return new Power();
}