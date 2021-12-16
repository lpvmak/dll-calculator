#include "error.h"

ErrorState::error_state_t ErrorState::calc_error = ErrorState::errorState::SUCCESS;

ErrorState::error_state_t ErrorState::getErrorState() {
	return calc_error;
}

void ErrorState::setErrorState(error_state_t const& error) {
	calc_error = error;
}

std::string ErrorState::getErrorMessage() {

	std::string message;

	switch (calc_error)
	{
	case ERROR_BRACKETS:
		message = "ERROR: Ошибка скобок";
		break;
	case ERROR_OPERATION:
		message = "ERROR: Операция не поддерживается\n";
		break;
	case ERROR_FUNCTION:
		message = "ERROR: Неизвестная функция\n";
		break;
	case ERROR_EMPTY_BRACKETS:
		message = "ERROR: Пустые скобки\n";
		break;
	case ERROR_POINT:
		message = "ERROR: Ошибка с плавующей точкой\n";
		break;
	default:
		break;
	}
	return message;
}

bool ErrorState::isSuccess() {
	if (getErrorState() == SUCCESS) return true;
	return false;
}
