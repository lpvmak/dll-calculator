#include "calculator.h"

typedef Operation* (*load)();

std::map<int, std::vector<Operation*>> Calculator::operations_map = {};
std::vector<Operation*> Calculator::constants = {};
std::vector<Operation*> Calculator::functions = {};


Calculator::Calculator(const char* path) {
    std::string _path = path;
    loadDllPlugins(_path);
    connectDllPlugins();
}

void Calculator::loadDllPlugins(std::string const &path) {
    std::filesystem::directory_iterator folder;
    try
    {
        folder = std::filesystem::directory_iterator(path);
    }
    catch (const std::exception& exc)
    {
        std::cerr << "Plugins not found" << std::endl << exc.what() << std::endl;
        return;
    }
    std::cout << "Connected plugins:" << std::endl;
    for (auto& dll : folder) {
        if (dll.path().extension().string() != ".dll") {
            continue;
        }

        HMODULE dlllib = LoadLibrary(dll.path().string().c_str());
        if (!dlllib) {
            std::cout << "error with connecting " << dll.path().filename().string() << std::endl;
            continue;
        }
        load loadOperation = (load)GetProcAddress(dlllib, "loadOperation");
        if (!loadOperation) {
            std::cout << "error with loading plugin " << dll.path().filename().string() << std::endl;
            FreeLibrary(dlllib);
            continue;
        }
        dll_libs.push_back(dlllib);
        plugins.push_back(loadOperation());
        std::cout << "\t- " << dll.path().filename().string() << std::endl;
    }
    std::cout << "\n";
}

void Calculator::connectDllPlugins() {
    for (auto& el : plugins) {
        switch (el->getType()) {
        case OperationType::CONSTANT:
            constants.push_back(el->getOperation());
            valid_functions.push_back(el->getName());
            break;
        case OperationType::BINARY:
            if (operations_map.find(el->getPrior()) == operations_map.end())
                operations_map.insert(std::make_pair(el->getPrior(), std::vector<Operation*>()));
            operations_map[el->getPrior()].push_back(el->getOperation());
            valid_operations.push_back(el->getName());

        case OperationType::FUNCTION:
            valid_functions.push_back(el->getName());
            functions.push_back(el->getOperation());
            break;
        default:
            break;
        }
    }
}

void Calculator::setExpression(std::string const &exp) {
    str_expression = exp;
    ErrorState::setErrorState(ErrorState::SUCCESS);
    processError();
    if (ErrorState::isSuccess()) {
        if (expression != nullptr) {
            free(expression);
        }
        expression = new Expression(exp);
    }
}

double Calculator::calculate(std::string &str) {
    str = unaryOperationsProcessing(str);
    setExpression(str);
    if (ErrorState::isSuccess()) {
        return  expression->calculate();
    }
    return -1;
}

std::string Calculator::getBinaryOperationLexems()
{
    std::string str = "";
    for (auto& pair : operations_map)
    {
        for (auto& op : pair.second)
        {
            str.append(op->getName());
        }
    }
    return str;
}

std::string Calculator::unaryOperationsProcessing(std::string &str) {
    int i = 0;
    str = replaceAll(str, " ", "");
    int size = str.length();
    while (str[i] == '-') { str[i++] = BaseLexems::unary_minus; }
    while (str[i] == '+') { str[i++] = BaseLexems::unary_plus; }
    
    for (i; i < size; i++) {
        if (strchr(getBinaryOperationLexems().c_str(), str[i])) {
            do {
                i++;
                if (str[i] == '-') { str[i] = BaseLexems::unary_minus; }
                else if (str[i] == '+') { str[i] = BaseLexems::unary_plus; }
                else break;
            } while (i < size - 1);
            i--;
        }
    }
    std::string newStr = replaceAll(str, "#", "");
    newStr = replaceAll(newStr, "__", "");
    newStr = replaceAll(newStr, "__", "");
    newStr = replaceAll(newStr, "_\\(_", "(");
    return newStr;
}


bool Calculator::processAlphaError(int *index){
    int startIndex = *(index);
    int currentIndex = *(index);
    while (isalpha(str_expression[currentIndex])) currentIndex++;
    if (!isLexemDefined(substr(str_expression, startIndex, currentIndex - startIndex))) {
        ErrorState::setErrorState(ErrorState::ERROR_FUNCTION);
        return true;
    }
    currentIndex--;
    *(index) = currentIndex;
    return false;
}

bool Calculator::processOperationError(int *index) {
    int startIndex = *(index);
    int currentIndex = *(index);
    while (!isalpha(str_expression[currentIndex]) && !isdigit(str_expression[currentIndex])
        && str_expression[currentIndex] != BaseLexems::lbracket && str_expression[currentIndex] != BaseLexems::rbracket && str_expression[currentIndex] != BaseLexems::unary_minus) {
        currentIndex++;
        if (currentIndex == str_expression.size()) {
            ErrorState::setErrorState(ErrorState::ERROR_OPERATION);
            return true;
        }
    }

    if (!isLexemDefined(substr(str_expression, startIndex, currentIndex - startIndex))) {
        ErrorState::setErrorState(ErrorState::ERROR_OPERATION);
        return true;
    }
    currentIndex--;
    *(index) = currentIndex;
    return false;
}

bool Calculator::isLexemDefined(std::string const &lexem) {
    if (isalpha(lexem[0])) return isFunctionDefined(lexem);
    else return isOperationDefined(lexem);
}

bool Calculator::isOperationDefined(std::string const &lexem) {
    for (auto& el : valid_operations) {
        if (!el.compare(lexem)) { return true; }
    }
    return isOperationUnaryMinus(lexem);
    return false;
} 

bool Calculator::isOperationUnaryMinus(std::string const &lexem) {
    if (lexem[0] == BaseLexems::unary_minus) {
        return true;
    }
    return false;
}

bool Calculator::isFunctionDefined(std::string const &lexem) {
    for (auto& el : valid_functions) {
        if (!el.compare(lexem)) { 
            return true; 
        }
    }
    return false;
}

bool Calculator::processExpressionBeg() {
    if (!isalpha(str_expression[0]) && !isdigit(str_expression[0])) {
        if (str_expression[0] != BaseLexems::lbracket&&str_expression[0] != BaseLexems::unary_minus) {
            ErrorState::setErrorState(ErrorState::ERROR_OPERATION);
            return true;
        }
    }
    return false;
}

bool Calculator::processPointError(symbol_t * previos, const int index) {
    if (*(previos) == BRACKET || *(previos) == ALPHA || *(previos) == FLOAT_POINT) {
        ErrorState::setErrorState(ErrorState::ERROR_POINT);
        return true;
    }
    if (!(index > 0 && isdigit(str_expression[index - 1])) && !(index < str_expression.size() - 1 && isdigit(str_expression[index + 1]))) {
        ErrorState::setErrorState(ErrorState::ERROR_POINT);
        return true;
    }
    *(previos) = FLOAT_POINT;
    return false;
}


void Calculator::processError() {
    int left_bracket = 0;
    int right_bracket = 0;
    symbol_t previos = NULLIC;

    bool isDigit = false;

    if (processExpressionBeg()) return;

    for (int i = 0; i < str_expression.size(); i++) {
        while (isdigit(str_expression[i])) {
            isDigit = true;
            if (previos == ALPHA|| previos==RIGHT_BRACKET) {
                ErrorState::setErrorState(ErrorState::ERROR_FUNCTION);
                return;
            }
            previos = DIGIT;
            i++;
        }
        if (right_bracket > left_bracket) {
            ErrorState::setErrorState(ErrorState::ERROR_BRACKETS);
            return;
        }

        if (i >= str_expression.size())  return;

        if (str_expression[i] == '.') {  if (processPointError(&previos, i)) return; } 
        if (str_expression[i] == '(') {
            if ( previos == DIGIT) {
                ErrorState::setErrorState(ErrorState::ERROR_BRACKETS);
                return;
            }
            isDigit = false;
            previos = LEFT_BRACKET;
            left_bracket++;
            continue;
        } 
        if (str_expression[i] == ')') {            
            right_bracket++;
            if (!isDigit && previos!=ALPHA) {
                ErrorState::setErrorState(ErrorState::ERROR_EMPTY_BRACKETS);
                return;
            }
            previos = RIGHT_BRACKET;
            continue;
        }
        if (isalpha(str_expression[i])) {
            previos = ALPHA;
            if (processAlphaError(&i)) return;
            continue;
        }
        if (isOperationUnaryMinus(&str_expression[i])) {
            if (i < str_expression.size() - 1) {
                if (isdigit(str_expression[i + 1]) || isalpha(str_expression[i + 1]) || str_expression[i + 1] == BaseLexems::lbracket) {
                    continue;
                }
            }
            ErrorState::setErrorState(ErrorState::ERROR_OPERATION);
            return;
        }
        else {
            if (processOperationError(&i)) return;
            previos = OPERATION;
                    
        }        
        
    }

    if (ErrorState::isSuccess()) {
        if (left_bracket != right_bracket) {
            ErrorState::setErrorState(ErrorState::ERROR_BRACKETS);
            return;
        }
    }
}



Calculator::~Calculator() {
    delete expression;
    for (auto& map_el : operations_map) {
        for (auto& vec_el : map_el.second) {
            delete vec_el;
        }
    }
    for (auto& vec_el : functions) {
        delete vec_el;
    }
    for (auto& vec_el : constants) {
        delete vec_el;
    }
    for (auto &lib : dll_libs) {
        FreeLibrary(lib);
    }
}
