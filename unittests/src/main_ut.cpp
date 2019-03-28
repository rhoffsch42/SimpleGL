#include "main_ut.hpp"

int		main(int ac, char **av)
{
	int detailsDepth = 99;
	if (ac == 2)
		detailsDepth = atoi(av[1]);

	LaunchTests<UnitTests::MathTests>			math;
	LaunchTests<UnitTests::PropertiesTests>		pp;

	std::cout << std::endl;
	UnitTests::printResult();
	return (0);
}
