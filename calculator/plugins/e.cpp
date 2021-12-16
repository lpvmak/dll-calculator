#include "e.h"

E::E() : Operation(OperationType::CONSTANT, "e") {}
double E::execute(double left, double right) {
    double num = exp(1);
    return num;
}
Operation* E::getOperation() {
    return new E();
}

Operation* loadOperation() {
    return new E();
}