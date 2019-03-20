#include <iostream>

#include "unittests.hpp"
#include "mathtests.hpp"

template <typename T>
class LaunchTests {
public:
	LaunchTests<T>() {
		T	*test = new T();
		test->setUp();
		test->testEverything();
		test->tearDown();
	}
};

int		main(int ac, char **av)
{
	int detailsDepth = 99;
	if (ac == 2)
		detailsDepth = atoi(av[1]);

	LaunchTests<MathTests>	math;

	std::cout << "\nTests passed: " << UT_OK << "  " << UnitTest::getTestPassed() << " / " << UnitTest::getTestAmount() << std::endl;;
	if (UnitTest::getTestFailed())
		std::cout << "Tests failed: " << UT_FAIL << "  " << UnitTest::getTestFailed() << std::endl;

	return (0);
}