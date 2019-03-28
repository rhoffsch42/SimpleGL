#pragma once

#include <iostream>

#include "unittests.hpp"
#include "math_tests.hpp"
#include "properties_tests.hpp"

template <typename T>
class LaunchTests {
public:
	LaunchTests<T>() {
		T	*test = new T();
		test->setUp();
		test->testEverything();
		test->tearDown();
		delete test;
	}
};
