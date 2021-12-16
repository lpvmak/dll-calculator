#include "number.h"

Number::Number(const std::string &num) {
    number = num;
    isNegative = false;
}

double Number::calculate() {
    double num = std::stod(number);
    return (isNegative) ? num * (-1) : num;
}
