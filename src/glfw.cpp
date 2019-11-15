#include "simplegl.h"
#include "glfw.hpp"

static void		defaultErrorCallback(int error, const char *description) {
	cerr << error << " : " << description << endl;
}

static void		defaultWindowCloseCallback(GLFWwindow* window) {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void		defaultMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	(void)window;(void)button;(void)action;(void)mods;
	// std::cout << __PRETTY_FUNCTION__ << std::endl;
	if (action == GLFW_PRESS) {
		GameManager * manager = static_cast<GameManager*>(glfwGetWindowUserPointer(window));
		if (manager->glfw && manager->glfw->cursorFree) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
				std::cout << "right button: ";
			else if (button == GLFW_MOUSE_BUTTON_LEFT)
				std::cout << "left button: ";
			std::cout << x << " : " << y << std::endl;
		}
	}
}

static void		defaultkeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void)window;(void)key;(void)scancode;(void)action;(void)mods;
	// std::cout << __PRETTY_FUNCTION__ << std::endl;
	if (false) {
		std::cout << "keys: " << key << std::endl;
		std::cout << "scancode: " << scancode << std::endl;
		std::cout << "action: " << action << std::endl;
		std::cout << "mods: " << mods << std::endl;
	}
	GameManager * manager = static_cast<GameManager*>(glfwGetWindowUserPointer(window));
	if (manager->glfw && manager->glfw->func[key])
		(manager->glfw->func[key])(window, key, scancode, action, mods);
}

static void		keyCallback_tab(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void)window;(void)key;(void)scancode;(void)action;(void)mods;
	std::cout << __PRETTY_FUNCTION__ << std::endl;

	if (action == GLFW_PRESS) {
		std::cout << "tab press" << std::endl;
		GameManager * manager = static_cast<GameManager*>(glfwGetWindowUserPointer(window));
		if (manager->glfw) {
			manager->glfw->toggleCursor();
		}
	}
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
	glfwSetErrorCallback(defaultErrorCallback);
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

	glfwMakeContextCurrent(this->_window);
	this->cursorFree = false;
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
	
	if (false) {
	std::cout << this->_mouseX << " : " << this->_mouseY \
	<< "\t(" << this->_mouseOriginX << " : " << this->_mouseOriginY << ")" << std::endl;
	} else if (false) {
		//diff
		std::cout <<this->_mouseOriginX - this->_mouseX << " : " \
				<< this->_mouseOriginY - this->_mouseY << std::endl;
	}

	if (!this->cursorFree) {
		this->_mouseDiffX = this->_mouseX - this->_mouseOriginX + this->_mouseOffsetX;
		this->_mouseDiffY = this->_mouseY - this->_mouseOriginY + this->_mouseOffsetY;

		if (this->_mouseWall >= 0) {
			if (this->_mouseDiffY > this->_mouseWall) {
				this->_mouseDiffY = this->_mouseWall;
				glfwSetCursorPos(this->_window, this->_mouseX, this->_mouseOriginY + this->_mouseWall - this->_mouseOffsetY);//hit the wall sooner due to offset
			} else if (this->_mouseDiffY < -this->_mouseWall) {
				this->_mouseDiffY = -this->_mouseWall;
				glfwSetCursorPos(this->_window, this->_mouseX, this->_mouseOriginY - this->_mouseWall - this->_mouseOffsetY);//hit the wall sooner due to offset
			}
		}
	}
}

void		Glfw::toggleCursor() {
	int	modes[2] = {GLFW_CURSOR_NORMAL, GLFW_CURSOR_DISABLED};
	glfwGetCursorPos(this->_window, &this->_mouseX, &this->_mouseY);
	if (this->cursorFree) {// if cursor is about to be locked

		this->_mouseOriginX = this->_mouseX;
		this->_mouseOriginY = this->_mouseY;
		/*
			As the origin is changed, we have to set the virtual cursor at the same diff as before (when we left locked state),
			we just save the diff in an offset that will be added on the later diff calculations
		*/
		this->_mouseOffsetX = this->_mouseDiffX;
		this->_mouseOffsetY = this->_mouseDiffY;
	}
	this->cursorFree = !this->cursorFree;//toggle
	glfwSetInputMode(this->_window, GLFW_CURSOR, modes[(this->cursorFree ? 0 : 1)]);
}

void		Glfw::activateDefaultCallbacks(GameManager * manager) {
	glfwSetWindowUserPointer(this->_window, manager);
	glfwSetWindowCloseCallback(this->_window, defaultWindowCloseCallback);
	glfwSetKeyCallback(this->_window, defaultkeyCallback);
	glfwSetMouseButtonCallback(this->_window, defaultMouseButtonCallback);
	
	// key map
	this->func[GLFW_KEY_TAB] = keyCallback_tab;
}

void		Glfw::setMouseAngle(double angle) {// set to negative to deactivate
	this->_mouseAngle = std::min(angle, (double)MOUSE_MAX_ANGLE);
	this->_mouseWall = this->_mouseAngle / (360.0 / MOUSE_SENSIBILITY);
}

void		Glfw::setTitle(std::string newTitle) {
	//verifier selon la taille de la fenetre? trunc ? a check
	this->_title = newTitle;
	glfwSetWindowTitle(this->_window, this->_title.c_str());
}

double		Glfw::getMouseAngle() const { return (this->_mouseAngle); }
std::string	Glfw::getTitle() const { return (this->_title); }
int			Glfw::getWidth() const { return (this->_width); }
int			Glfw::getHeight() const { return (this->_height); }
