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

class Redirecter
{
public:
	Redirecter(std::stringstream& dst, std::ostream& src)
		: dst(dst), src(src), old(src.rdbuf()) {
		src.rdbuf(dst.rdbuf());
	}
	void	restore() { src.rdbuf(old); }
	std::string	getContent() { return dst.str(); }
	~Redirecter() { }
	std::ostream&	src;
	std::streambuf* old;
	std::stringstream& dst;
private:
};

struct buffer_restorer {
	std::ios& m_s;
	std::streambuf* m_buf;

	buffer_restorer(std::ios& s, std::streambuf* buf) : m_s(s), m_buf(buf) {}
	void	restore() { m_s.rdbuf(m_buf); }
	~buffer_restorer() { }
};

//#pragma execution_character_set( "utf-8" )
#include <ostream>
int		main(int ac, char **av)
{
	std::cout << Misc::getCurrentDirectory() << std::endl;
	std::cout << "Warning: this program needs the file images/lena.bmp\n\tBe careful with the current directory!" << std::endl;

	if (SetConsoleOutputCP(65001) == 0)	{
		std::cout << "fuck SetConsoleOutputCP\n";
		exit(0);
	}
	int detailsDepth = 99;
	if (ac == 2)
		detailsDepth = atoi(av[1]);

	std::cout << UT_OK << UT_ERROR << UT_FAIL << UT_HUMAN << " \n";
	std::stringstream  trashstream;
	//Redirecter redirect(trashstream, std::cout);
	buffer_restorer redirect(std::cout, std::cout.rdbuf(trashstream.rdbuf()));

	launchTests<UnitTests::MathTests>();
	launchTests<UnitTests::PropertiesTests>();
	launchTests<UnitTests::GlfwTests>();
	launchTests<UnitTests::TextureTests>();
	launchTests<UnitTests::ObjectTests>();
	launchTests<UnitTests::BehaviorManagedTests>();
	launchTests<UnitTests::BehaviorTests>();
	/*
		Tests:
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

	//std::cout.rdbuf(redirect.old);
	redirect.restore();
	UnitTests::printResult();
	//std::cout << redirect.getContent() << std::endl;

	std::cout << "end\n";
	return (0);
}