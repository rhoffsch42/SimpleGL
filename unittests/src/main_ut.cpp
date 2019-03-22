#include "main_ut.hpp"

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

int		main(int ac, char **av)
{
	int detailsDepth = 99;
	if (ac == 2)
		detailsDepth = atoi(av[1]);

	LaunchTests<MathTests>	math;

	std::cout << std::endl;
	UnitTest::printResult();
	return (0);
}
