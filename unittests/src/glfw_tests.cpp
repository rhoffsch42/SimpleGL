#include "glfw_tests.hpp"

void	UnitTests::GlfwTests::setUp() {
	min_default_width = 100;			max_default_width = 800;
	min_default_height = 100;			max_default_height = 600;
	min_width = 100;
	min_height = 100;
	widthA_1920 = 1920;
	heightA_1080 = 1080;
	angleA_45 = 45;
	angleB_150 = 150;
	title = "A random title";
}
void	UnitTests::GlfwTests::tearDown() {
}
void	UnitTests::GlfwTests::testEverything() const {
	testGlfwConstants();
	testGlfw();
	testGlfw_updateMouse();
	testGlfw_setMouseAngle();
	testGlfw_setTitle();
	testGlfw_getMouseAngle();
	testGlfw_getTitle();
}

void	UnitTests::GlfwTests::testGlfwConstants() const {
	TEST("Glfw() constants", DEFAULT_WIDTH >= min_default_width && DEFAULT_WIDTH <= max_default_width);
	TEST("Glfw() constants", DEFAULT_HEIGHT >= min_default_height && DEFAULT_HEIGHT <= max_default_height);
	TEST("Glfw() constants", MIN_WIDTH >= min_width);
	TEST("Glfw() constants", MIN_HEIGHT >= min_height);
}
void	UnitTests::GlfwTests::testGlfw() const {
	Glfw	gl1;
	TEST("Glfw()", gl1._width == DEFAULT_WIDTH);
	TEST("Glfw()", gl1._height == DEFAULT_HEIGHT);
	TEST("Glfw()", gl1._title == DEFAULT_TITLE);
	TEST("Glfw()", gl1._window != NULL);
	glfwDestroyWindow(gl1._window);

	Glfw	gl2(0, 0);
	TEST("Glfw(0, 0)", gl2._width == MIN_WIDTH);
	TEST("Glfw(0, 0)", gl2._height == MIN_HEIGHT);
	TEST("Glfw(0, 0)", gl2._title == DEFAULT_TITLE);
	TEST("Glfw(0, 0)", gl2._window != NULL);
	glfwDestroyWindow(gl2._window);

	Glfw	gl3(widthA_1920, heightA_1080);
	TEST("Glfw(1920, 1080)", gl3._width == widthA_1920);
	TEST("Glfw(1920, 1080)", gl3._height == heightA_1080);
	TEST("Glfw(1920, 1080)", gl3._title == DEFAULT_TITLE);
	TEST("Glfw(1920, 1080)", gl3._window != NULL);
	glfwDestroyWindow(gl3._window);
}
void	UnitTests::GlfwTests::testGlfw_updateMouse() const {
	NOT_TESTED("Glfw updateMouse", " : need human touch (check the cursor visually)", UnitTests::test_human);
}
void	UnitTests::GlfwTests::testGlfw_setMouseAngle() const {
	Glfw	gl4(widthA_1920, heightA_1080);

	gl4.setMouseAngle(angleB_150);
	TEST("Glfw setMouseAngle(150)", gl4._mouseAngle == MOUSE_MAX_ANGLE);
	gl4.setMouseAngle(angleA_45);
	TEST("Glfw setMouseAngle(45)", gl4._mouseAngle == angleA_45);
	NOT_TESTED("Glfw setMouseAngle", " : need to check _mouseWall, see calculation in glfw.cpp", UnitTests::test_human);
	glfwDestroyWindow(gl4._window);
}
void	UnitTests::GlfwTests::testGlfw_setTitle() const {
	Glfw	gl5(widthA_1920, heightA_1080);

	gl5.setTitle(title);
	TEST("Glfw setTitle(\"A random title\")", gl5._title == title);
	NOT_TESTED("Glfw setTitle(\"A random title\")", " : need human touch (check window title visually)", UnitTests::test_human);
}
void	UnitTests::GlfwTests::testGlfw_getMouseAngle() const {
	Glfw	gl6(widthA_1920, heightA_1080);

	double angle = gl6.getMouseAngle();
	TEST("Glfw getMouseAngle", angle == gl6._mouseAngle);
	TEST("Glfw getMouseAngle", &angle != &gl6._mouseAngle);
}
void	UnitTests::GlfwTests::testGlfw_getTitle() const {
	Glfw	gl7(widthA_1920, heightA_1080);

	std::string	title1 = gl7.getTitle();
	TEST("Glfw getMouseAngle", title1 == gl7._title);
	TEST("Glfw getMouseAngle", &title1 != &gl7._title);
}