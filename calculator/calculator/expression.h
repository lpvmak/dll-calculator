#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "expression_interface.h"
#include "operation.h"
#include "common.h"

class Expression: public ExpressionInterface {
private:
    ExpressionInterface* left;
    ExpressionInterface* right;

protected:
    std::string operation;
    std::string full;

public:
    Expression();
    Expression(const std::string  &str);
    ~Expression();

    Expression *getExpression();
    double calculate();    

protected:
    void processExpression(const std::string &str, int index);
    void addExpression(std::string str, ExpressionInterface *& place);

private:
    void parseExpression(const std::string &str);
    void processBinaryOperations(const std::string& str, bool* isActivated);
    void processPower(const std::string &str, bool * isActivated);
    void processSecondPriorityOperations(const std::string &str, bool * isActivated);
    void processFirstPriorityOperations(const std::string &str, bool * isActivated);
    void processBrackets(const std::string &str, bool * isActivated);
    Operation * defineFunctions(std::string str);
    Operation * defineConstant(std::string str);
};

#endif //EXPRESSION_H
