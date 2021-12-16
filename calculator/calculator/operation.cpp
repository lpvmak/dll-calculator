#include "operation.h"

Operation::Operation(OperationType const &operationType, std::string const &operationName, int prior) {
    _type = operationType;
    _name = operationName;
    _prior = prior;
}

OperationType Operation::getType() {
    return _type;
}

std::string Operation::getName() {
    return _name;
}

int Operation::getPrior()
{
    return _prior;
}

char BaseLexems::unary_minus = '_';
char BaseLexems::unary_plus = '#';
char BaseLexems::lbracket = '(';
char BaseLexems::rbracket = ')';

