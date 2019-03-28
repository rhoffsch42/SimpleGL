#include "unittests.hpp"

unsigned int UnitTests::_test_amount = 0;
unsigned int UnitTests::_test_passed = 0;
unsigned int UnitTests::_test_failed = 0;
unsigned int UnitTests::_test_errors = 0;

UnitTests::UnitTests(){
}

void			UnitTests::printResult() {
	std::cout << "Tests passed: " << UT_OK << "  " << UnitTests::getTestPassed() << " / " << UnitTests::getTestAmount() << std::endl;;
	if (UnitTests::getTestFailed())
		std::cout << "Tests failed: " << UT_FAIL << "  " << UnitTests::getTestFailed() << std::endl;
	if (UnitTests::getTestErrors())
		std::cout << "Tests errors: " << UT_ERROR << "  " << UnitTests::getTestErrors() << std::endl;
}

std::string		UnitTests::test_ok() {
	UnitTests::_test_amount++;
	UnitTests::_test_passed++;
	return (UT_OK);
}

std::string		UnitTests::test_fail() {
	UnitTests::_test_amount++;
	UnitTests::_test_failed++;
	return (UT_FAIL);
}

std::string		UnitTests::test_error() {
	UnitTests::_test_amount++;
	UnitTests::_test_errors++;
	return (UT_ERROR);
}

unsigned int	UnitTests::getTestAmount() { return UnitTests::_test_amount; }
unsigned int	UnitTests::getTestPassed() { return UnitTests::_test_passed; }
unsigned int	UnitTests::getTestFailed() { return UnitTests::_test_failed; }
unsigned int	UnitTests::getTestErrors() { return UnitTests::_test_errors; }

std::string		UnitTests::padded(std::string str, int n) {
	std::string pad(str); pad.resize(n, ' ');\
	return pad;
}
