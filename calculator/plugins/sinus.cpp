#include "sinus.h"

Sinus::Sinus() : Operation(OperationType::FUNCTION, "sin") {}
double Sinus::execute(double left, double right) {
    double num = sin(left);
    return num;
}
Operation* Sinus::getOperation() {
    return new Sinus();
}

Operation *loadOperation() {
    return new Sinus();
}