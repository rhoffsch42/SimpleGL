#include "unittests.hpp"

unsigned int UnitTest::_test_amount = 0;
unsigned int UnitTest::_test_passed = 0;
unsigned int UnitTest::_test_failed = 0;
unsigned int UnitTest::_test_errors = 0;

UnitTest::UnitTest(){
}

void			UnitTest::printResult() {
	std::cout << "Tests passed: " << UT_OK << "  " << UnitTest::getTestPassed() << " / " << UnitTest::getTestAmount() << std::endl;;
	if (UnitTest::getTestFailed())
		std::cout << "Tests failed: " << UT_FAIL << "  " << UnitTest::getTestFailed() << std::endl;
	if (UnitTest::getTestErrors())
		std::cout << "Tests errors: " << UT_ERROR << "  " << UnitTest::getTestErrors() << std::endl;
}

std::string		UnitTest::test_ok() {
	UnitTest::_test_amount++;
	UnitTest::_test_passed++;
	return (UT_OK);
}

std::string		UnitTest::test_fail() {
	UnitTest::_test_amount++;
	UnitTest::_test_failed++;
	return (UT_FAIL);
}

std::string		UnitTest::test_error() {
	UnitTest::_test_amount++;
	UnitTest::_test_errors++;
	return (UT_ERROR);
}

unsigned int	UnitTest::getTestAmount() { return UnitTest::_test_amount; }
unsigned int	UnitTest::getTestPassed() { return UnitTest::_test_passed; }
unsigned int	UnitTest::getTestFailed() { return UnitTest::_test_failed; }
unsigned int	UnitTest::getTestErrors() { return UnitTest::_test_errors; }

std::string		UnitTest::padded(std::string str, int n) {
	std::string pad(str); pad.resize(n, ' ');\
	return pad;
}
