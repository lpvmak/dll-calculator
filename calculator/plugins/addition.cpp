#include "addition.h"

Addition::Addition() : Operation(OperationType::BINARY, "+", 1) {}

double Addition::execute(double left, double right) {
    double num = (left + right);
    return num;
}

Operation* Addition::getOperation() {
    return new Addition();
}

Operation *loadOperation() {
    return new Addition();
}