#include "main_ut.hpp"

template <typename T>
void	launchTests() {
	T	*test = new T();
	test->setUp();
	test->testEverything();
	test->tearDown();
	delete test;
	UnitTests::streamTests << std::endl;
}

int		main(int ac, char **av)
{
	int detailsDepth = 99;
	if (ac == 2)
		detailsDepth = atoi(av[1]);

	std::streambuf		*old = std::cout.rdbuf(); // <-- save
	std::stringstream	tested_output;
	std::cout.rdbuf(tested_output.rdbuf());// <-- redirect

	launchTests<UnitTests::MathTests>();
	launchTests<UnitTests::PropertiesTests>();
	launchTests<UnitTests::GlfwTests>();
	launchTests<UnitTests::TextureTests>();
	launchTests<UnitTests::ObjectTests>();
	launchTests<UnitTests::BehaviorManagedTests>();
	// launchTests<UnitTests::BehaviorTests>();
	/*
		Tests:
			refacto Math
			Behavior
			TransformBH : Behavior
			Cam
			Obj3dBP
			Program
			SkyboxPG
			Obj3dPG
			Blueprint
			Skybox
			Obj3D
		end

	*/

	std::cout.rdbuf(old);// <-- restore
	std::cout << std::endl;
	UnitTests::printResult();
	// std::cout << tested_output.str();

	return (0);
}
