#pragma once

#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>

#define	ASSERT_MODE		false
#define UT_OK			"\xe2\x9c\x85" // ✅
#define UT_FAIL			"\xe2\x9d\x8c" // ❌
#define UT_ERROR		"\xe2\x9a\xa0\xef\xb8\x8f" // ⚠️
#define UT_HUMAN		"\xf0\x9f\x91\x81" // 👁
#define UT_PAD			40

#define NOT_TESTED(title, comment, func)	UnitTests::streamTests << func() << "\t" << UnitTests::padded(title, UT_PAD) << "\t\t" << "Not tested" << comment << std::endl
#define TEST(title, cond)	if (ASSERT_MODE) {assert(cond);}\
					UnitTests::streamTests << ((cond) ? UnitTests::test_ok() : UnitTests::test_fail()) << "\t" << UnitTests::padded(title, UT_PAD) << "\t\t" << #cond << std::endl

class UnitTests {
public:
	static std::stringstream	streamTests;

	static void				printResult();

	static std::string		test_ok();
	static std::string		test_fail();
	static std::string		test_error();
	static std::string		test_human();
	static unsigned int		getTestAmount();
	static unsigned int		getTestPassed();
	static unsigned int		getTestFailed();
	static unsigned int		getTestErrors();
	static unsigned int		getTestHuman();
	// util:
	static std::string		padded(std::string str, int n);
	// tests classes
	class MathTests;
	class PropertiesTests;
	class GlfwTests;
	class TextureTests;
	class ObjectTests;
	class BehaviorManagedTests;
protected:
	UnitTests();
private:
	static unsigned int		_test_amount;
	static unsigned int		_test_passed;
	static unsigned int		_test_failed;
	static unsigned int		_test_errors;
	static unsigned int		_test_human;
};

// include AFTER the declaration of UnitTests to resolve incomplete Types (tests classes)
#include "math_tests.hpp"
#include "properties_tests.hpp"
#include "glfw_tests.hpp"
#include "texture_tests.hpp"
#include "object_tests.hpp"
#include "behaviormanaged_tests.hpp"
