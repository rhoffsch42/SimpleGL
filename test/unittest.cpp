#include <iostream>

///////////////////////////////////////////////////////////////// unittest.hpp
#include <cassert>

#define	ASSERT_MODE		false
#define UT_OK			"\xe2\x9c\x85" // ✅
#define UT_FAIL			"\xe2\x9d\x8c" // ❌

#define TEST(cond)	if (ASSERT_MODE) {assert(cond);}\
					std::cout << ((cond) ? UnitTest::test_ok() : UnitTest::test_fail()) << "\t(" << #cond << ") " << std::endl;

class UnitTest {
public:
	static std::string		test_ok() {
		_test_amount++;
		_test_passed++;
		return (UT_OK);
	}
	static std::string		test_fail() {
		_test_amount++;
		_test_failed++;
		return (UT_FAIL);
	}
	static unsigned int	getTestAmount() { return _test_amount; }
	static unsigned int	getTestPassed() { return _test_passed; }
	static unsigned int	getTestFailed() { return _test_failed; }
private:
	static unsigned int _test_amount;
	static unsigned int _test_passed;
	static unsigned int _test_failed;
};
unsigned int UnitTest::_test_amount = 0;
unsigned int UnitTest::_test_passed = 0;
unsigned int UnitTest::_test_failed = 0;

///////////////////////////////////////////////////////////////// main.c
class Base {
public:
	Base(int a) : _n(a) {}
	int get(void) { return this->_n; }
private:
	int _n;
};

int		main(void)
{
	TEST(1 == 1);
	TEST(1 == 2);

	Base	b1(1);
	Base	b5(5);
	TEST(b1.get() == b5.get());

	std::cout << "\nTests passed: " << UT_OK << "  " << UnitTest::getTestPassed() << " / " << UnitTest::getTestAmount() << std::endl;;
	if (UnitTest::getTestFailed())
		std::cout << "Tests failed: " << UT_FAIL << "  " << UnitTest::getTestFailed() << std::endl;
	return (0);
}