#include "main_ut.hpp"

template <typename T>
void	launchTests() {
	T	*test = new T();
	test->setUp();
	test->testEverything();
	test->tearDown();
	delete test;
	std::cout << std::endl;
}

int		main(int ac, char **av)
{
	int detailsDepth = 99;
	if (ac == 2)
		detailsDepth = atoi(av[1]);

	launchTests<UnitTests::MathTests>();
	launchTests<UnitTests::PropertiesTests>();
	launchTests<UnitTests::GlfwTests>();
	launchTests<UnitTests::TextureTests>();

	std::cout << std::endl;
	UnitTests::printResult();
	return (0);
}
