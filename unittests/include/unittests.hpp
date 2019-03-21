#pragma once

#include <cassert>
#include <iostream>
#include <iomanip>

#define	ASSERT_MODE		false
#define UT_OK			"\xe2\x9c\x85" // ✅
#define UT_FAIL			"\xe2\x9d\x8c" // ❌
#define UT_ERROR		"\xe2\x9a\xa0\xef\xb8\x8f" // ⚠️
#define UT_PAD				35

#define NOT_TESTED(title, comment)	std::cout << UnitTest::test_error() << "\t" << UnitTest::padded(title, UT_PAD) << "\t\t" << "Not tested" << comment << std::endl
#define TEST(title, cond)	if (ASSERT_MODE) {assert(cond);}\
					std::cout << ((cond) ? UnitTest::test_ok() : UnitTest::test_fail()) << "\t" << UnitTest::padded(title, UT_PAD) << "\t\t" << #cond << std::endl

class UnitTest {
public:
	static std::string		test_ok();
	static std::string		test_fail();
	static std::string		test_error();
	static unsigned int		getTestAmount();
	static unsigned int		getTestPassed();
	static unsigned int		getTestFailed();
	static unsigned int		getTestErrors();
	//util:
	static std::string		padded(std::string str, int n);
protected:
	UnitTest();
private:
	static unsigned int		_test_amount;
	static unsigned int		_test_passed;
	static unsigned int		_test_failed;
	static unsigned int		_test_errors;
};
