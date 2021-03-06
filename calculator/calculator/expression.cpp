#include "expression.h"
#include "function.h"
#include "calculator.h"
#include "number.h"

Expression::Expression() {
    isNegative = false;
    isUnary = false;
}

Expression::~Expression() {
    delete left;
    delete right;
}

Operation* Expression::defineFunctions(std::string str) {
    std::string tmp;
    std::vector<Operation*> expressions = Calculator::functions;
    for (int i = 0;i < str.size();i++) {
        if (str[i] == BaseLexems::lbracket) {
            tmp = substr(str, 0, i);
            break;
        }
    }
    for (auto& el : expressions) {        
        if (!(el->getName()).compare(tmp)) {
           return el->getOperation();
        }
    }
    return nullptr;
}

Operation* Expression::defineConstant(std::string str) {
    std::vector<Operation*> expressions = (Calculator::constants);    
    for (auto& el : expressions) {
        if (!(el->getName()).compare(str)) {
            return el->getOperation();
        }
    }
    return nullptr;
}

Expression::Expression(const std::string  &str){
    full = str;
    isNegative = false;
    parseExpression(str);
}

Expression* Expression:: getExpression() {
    return this;
}

double Expression::calculate() {
    double num;
    if (base_operations == nullptr) {
        num = left->calculate();
    }
    else {
        if (!(right == nullptr)) { 
            num = base_operations->execute(left->calculate(), right->calculate());
        } else num = base_operations->execute(left->calculate(), 0);
    }
    return (isNegative) ? num * (-1) : num;
}


void Expression::parseExpression(const std::string &str) {    
    bool isRecurtionActivated = false;

    processBrackets(str, &isRecurtionActivated);
    if (isRecurtionActivated) return;  
    processBinaryOperations(str, &isRecurtionActivated);
    if (isRecurtionActivated) return;
    processExpression(str, str.length());
}


void Expression::processBinaryOperations(const std::string& str, bool* isActivated) {
    int brackets = 0;
    std::string tmp;
    for (const auto& operation_pair : Calculator::operations_map) {
        std::vector<Operation*> expressions = operation_pair.second;
        for (int i = str.length() - 1; i >= 0; i--) {
            tmp = str[i];
            if (brackets == 0) {
                for (auto& el : expressions) {
                    if (!(el->getName()).compare(tmp)) {
                        base_operations = el->getOperation();
                        if (str[0] == BaseLexems::unary_minus) {
                            isNegative = true;
                            processExpression(substr(str, 1, str.length() - 1), i - 1);
                        }
                        else
                            processExpression(str, i);
                        *isActivated = true;
                        return;
                    }
                }
            }
            if (str[i] == BaseLexems::lbracket) brackets++;
            if (str[i] == BaseLexems::rbracket) brackets--;
        }
        if (*isActivated)
            return;
    }
}


void Expression::processBrackets(const std::string &str, bool *isActivated) {
    if (str[0] == BaseLexems::lbracket && str[str.length() - 1] == BaseLexems::rbracket && brack(substr(str, 1, str.length() - 2))) {
        *isActivated = true;
        return parseExpression(substr(str, 1, str.length() - 2));
    } else if (str[0] == BaseLexems::unary_minus && str[1] == BaseLexems::lbracket && 
               str[str.length() - 1] == BaseLexems::rbracket && brack(substr(str, 2, str.length() - 3))) {
        isNegative = true;
        *isActivated = true;
        return parseExpression(substr(str, 2, str.length() - 3));
    }    
}


void Expression::processExpression(const std::string &str, int index) {
    std::string substrl = substr(str, 0, index);
    std::string substrr = substr(str, index + 1, str.length() - index - 1);
    if(!substrl.empty()) addExpression(substrl, this->left);
    if(!substrr.empty()) addExpression(substrr, this->right);
    else isUnary = true;
    operation = str[index];
}


void Expression::addExpression(std::string str, ExpressionInterface *&place) {

    std::regex unary_minus("_");
    std::regex right_bracket("\\(");
    std::regex left_bracket("\\)");
    Operation *opertmp;
    bool first_minus = false;
    bool minus_after_bracket = false;
    std::string func;
    if (str[0] == BaseLexems::unary_minus) {
        str = substr(str, 1, str.length() - 1);
        first_minus = true;
    }
    std::string tmp = str;
    if (str[0] == BaseLexems::lbracket) {
        tmp = substr(tmp, 1, tmp.length() - 2);
        if (str[1] == BaseLexems::unary_minus) {
            tmp = std::regex_replace(tmp, unary_minus, "");
            minus_after_bracket = true;
        }
    }
    opertmp = defineFunctions(str);
    if (opertmp) {
        int i = 0;
        while (isalpha(tmp[i])) i++;
        for (i;i < tmp.size();i++) {
            if (str[i] == BaseLexems::rbracket) {
                break;
            }
        }
        if (i == tmp.size()-1) {
            place = new Function(tmp, opertmp);
            place->isNegative = first_minus;            
            return;
        }
    }
    opertmp = defineConstant(str);
    if (opertmp) {
        place = new Constant(opertmp);
        place->isNegative = first_minus;
        return;
    }
    if (isNumber(tmp)) {
        tmp = std::regex_replace(tmp, unary_minus, "");
        place = new Number(tmp);
        if (first_minus || minus_after_bracket) {
            place->isNegative = true;
        }
    }
    else {
        place = new Expression(str);
        place->isNegative = first_minus;
    }
}
