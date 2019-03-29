#pragma once

#include "unittests.hpp"
#include "glfw.hpp"
#include <sstream>

class UnitTests::GlfwTests {
public:
	void	setUp();
	void	tearDown();
	void	testEverything() const;

	void	testGlfwConstants() const;
	void	testGlfw() const;
	void	testGlfw_updateMouse() const;
	void	testGlfw_setMouseAngle() const;
	void	testGlfw_setTitle() const;
	void	testGlfw_getMouseAngle() const;
	void	testGlfw_getTitle() const;
	// void	testGlfw_() const;
private:
	unsigned int	min_default_width;
	unsigned int	max_default_width;
	unsigned int	min_default_height;
	unsigned int	max_default_height;
	unsigned int	min_width;
	unsigned int	min_height;
	unsigned int	widthA_1920;
	unsigned int	heightA_1080;
	double			angleA_45;
	double			angleB_150;
	std::string		title;
};