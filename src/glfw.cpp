#include "simplegl.h"
#include "glfw.hpp"

static void		errorCallback(int error, const char *description) {
	cerr << error << " : " << description << endl;
}

static void		windowCloseCallback(GLFWwindow* window) {
//	if (!time_to_close)
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Glfw::Glfw() {
	this->_width = DEFAULT_WIDTH;
	this->_height = DEFAULT_HEIGHT;
	this->_title = DEFAULT_TITLE;
	this->init();
}

Glfw::Glfw(unsigned int width, unsigned int height) {
	this->_width = std::min(width, (unsigned int)MAX_WIDTH);
	this->_width = std::max(width, (unsigned int)MIN_WIDTH);
	this->_height = std::min(height, (unsigned int)MAX_HEIGHT);
	this->_height = std::max(height, (unsigned int)MIN_HEIGHT);
	this->_title = DEFAULT_TITLE;
	this->init();
}

Glfw::~Glfw() {
}

void	Glfw::init() {
	//glfw
	if (!glfwInit()) {
		cerr << "glfwInit failed" << endl;
		Misc::breakExit(GL_ERROR);
	}
	glfwSetErrorCallback(errorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MINOR);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	this->_window = glfwCreateWindow(this->_width, this->_height, this->_title.c_str(), NULL, NULL);
	if (!this->_window) {
		cerr << "glfwCreateWindow failed" << endl;
		Misc::breakExit(GL_ERROR);
	}
	glfwSetWindowCloseCallback(this->_window, windowCloseCallback);
	
	glfwMakeContextCurrent(this->_window);
	glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(this->_window, GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(this->_window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	//glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "glewInit failed" << endl;
		Misc::breakExit(GL_ERROR);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(-1.0f);
	glDepthFunc(GL_GREATER);
	glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glfwSetCursorPos(this->_window, this->_width / 2, this->_height / 2);
	glfwGetCursorPos(this->_window, &this->_mouseOriginX, &this->_mouseOriginY);
	this->setMouseAngle(MOUSE_MAX_ANGLE);
	cout << "GL version: " << glGetString(GL_VERSION) << endl;
}

void	Glfw::updateMouse() {
	glfwGetCursorPos(this->_window, &this->_mouseX, &this->_mouseY);
	this->_mouseDiffX = float(this->_mouseOriginX - this->_mouseX);
	this->_mouseDiffY = float(this->_mouseOriginY - this->_mouseY);
	if (this->_mouseWall >= 0) {
		if (this->_mouseDiffY > this->_mouseWall) {
			this->_mouseDiffY = this->_mouseWall;
			glfwSetCursorPos(this->_window, this->_mouseX, this->_mouseOriginY - this->_mouseWall);
		}
		if (this->_mouseDiffY < -this->_mouseWall) {
			this->_mouseDiffY = -this->_mouseWall;
			glfwSetCursorPos(this->_window, this->_mouseX, this->_mouseOriginY + this->_mouseWall);
		}
	}
}

void	Glfw::setMouseAngle(double angle) {// set to negative to deactivate
	this->_mouseAngle = std::min(angle, (double)MOUSE_MAX_ANGLE);
	this->_mouseWall = this->_mouseAngle / (360.0 / MOUSE_SENSIBILITY);
}

void	Glfw::setTitle(std::string newTitle) {
	//verifier selon la taille de la fenetre? trunc ? a check
	this->_title = newTitle;
	glfwSetWindowTitle(this->_window, this->_title.c_str());
}

double		Glfw::getMouseAngle() const { return (this->_mouseAngle); }
std::string	Glfw::getTitle() const { return (this->_title); }
