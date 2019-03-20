#pragma once

#include <cassert>
#include <iostream>

#define	ASSERT_MODE		false
#define UT_OK			"\xe2\x9c\x85" // ✅
#define UT_FAIL			"\xe2\x9d\x8c" // ❌

#define TEST(cond)	if (ASSERT_MODE) {assert(cond);}\
					std::cout << ((cond) ? UnitTest::test_ok() : UnitTest::test_fail()) << "\t(" << #cond << ") " << std::endl;

class UnitTest {
public:
	static std::string		test_ok();
	static std::string		test_fail();
	static unsigned int		getTestAmount();
	static unsigned int		getTestPassed();
	static unsigned int		getTestFailed();
protected:
	UnitTest();
private:
	static unsigned int		_test_amount;
	static unsigned int		_test_passed;
	static unsigned int		_test_failed;
};
