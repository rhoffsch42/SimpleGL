#include "unittests.hpp"

unsigned int UnitTest::_test_amount = 0;
unsigned int UnitTest::_test_passed = 0;
unsigned int UnitTest::_test_failed = 0;

UnitTest::UnitTest(){
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

unsigned int	UnitTest::getTestAmount() { return UnitTest::_test_amount; }
unsigned int	UnitTest::getTestPassed() { return UnitTest::_test_passed; }
unsigned int	UnitTest::getTestFailed() { return UnitTest::_test_failed; }
