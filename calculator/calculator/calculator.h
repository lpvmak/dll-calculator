#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "operation.h"
#include "expression.h"
#include "error.h"
#include "common.h"
#include <Windows.h>
#include <filesystem>


class Calculator {
private:
    typedef enum symbol {
        DIGIT,
        FLOAT_POINT,
        ALPHA,
        BRACKET,
        RIGHT_BRACKET,
        LEFT_BRACKET,
        NULLIC,
        OPERATION,
    } symbol_t;

    std::vector<HMODULE> dll_libs;
    
    std::vector<Operation*> plugins;

    std::vector<std::string> valid_operations;
   
    std::vector<std::string> valid_functions;

    void loadDllPlugins(std::string const & path);
    
    void connectDllPlugins();

    void setExpression(std::string const & exp);

    bool processExpressionBeg();

    bool processPointError(symbol_t * previos, const int index);
    
    void processError();
    
    bool processAlphaError(int *index);
    
    bool processOperationError(int * index);
    
    bool isLexemDefined(std::string const & lexem);
    
    bool isOperationDefined(std::string const & lexem);
    
    bool isOperationUnaryMinus(std::string const & lexem);
    
    bool isFunctionDefined(std::string const & lexem);

    std::string unaryOperationsProcessing(std::string &str);
 
public:
    Calculator(const char* path = "..\\plugins\\");
    ~Calculator();

    std::string str_expression;
    Expression* expression;

    double calculate(std::string& str);
    
    static std::vector<Operation*> constants;
    
    static std::map<int, std::vector<Operation*>> operations_map;
    
    static std::vector<Operation*> functions;

    static std::string getBinaryOperationLexems();
};

#endif //CALCULATOR_H