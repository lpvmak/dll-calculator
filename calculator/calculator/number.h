#ifndef NUMBER_H
#define NUMBER_H

#include "expression_interface.h"
#include <string> 


class Number : public ExpressionInterface {
    std::string number;
public:
    Number(const std::string &num);

    virtual double calculate();
};


#endif //NUMBER_H
