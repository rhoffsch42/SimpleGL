#include "glfw.hpp"
#include "compiler_settings.h"
#include "simplegl.h"

#ifdef SGL_DEBUG
 #define SGL_GLFW_DEBUG
#endif
#ifdef SGL_GLFW_DEBUG 
 #define D(x) std::cout << "[Glfw] " << x ;
 #define D_(x) x
 #define D_SPACER "-- glfw.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

static void		defaultErrorCallback(int error, const char *description) {
	std::cout << error << " : " << description << "\n";
}

static void		defaultWindowCloseCallback(GLFWwindow* window) {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void		defaultMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	(void)window; (void)button; (void)action; (void)mods;
	//D(__PRETTY_FUNCTION__ << "\n")
	if (action == GLFW_PRESS) {
		GameManager* manager = static_cast<GameManager*>(glfwGetWindowUserPointer(window));
		if (manager->glfw && manager->glfw->cursorFree) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			if (button == GLFW_MOUSE_BUTTON_RIGHT)		{	D("right button: ") }
			else if (button == GLFW_MOUSE_BUTTON_LEFT)	{	D("left button: ") }
			D(x << " : " << y << "\n");
		}
	}
}

static void		defaultkeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void)window; (void)key; (void)scancode; (void)action; (void)mods;
	//D(__PRETTY_FUNCTION__ << "\n")
	if (false) {
		D("keys: " << key << "\n")
		D("scancode: " << scancode << "\n")
		D("action: " << action << "\n")
		D("mods: " << mods << "\n")
	}
	GameManager* manager = static_cast<GameManager*>(glfwGetWindowUserPointer(window));
	if (manager->glfw && manager->glfw->func[key])
		(manager->glfw->func[key])(window, key, scancode, action, mods);
}

static void		keyCallback_tab(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void)window; (void)key; (void)scancode; (void)action; (void)mods;
	D(__PRETTY_FUNCTION__ << "\n")

	if (action == GLFW_PRESS) {
		D("tab press\n")
		GameManager* manager = static_cast<GameManager*>(glfwGetWindowUserPointer(window));
		if (manager->glfw) {
			manager->glfw->toggleCursor();
		}
	}
}

std::thread::id Glfw::thread_id = std::this_thread::get_id();
void	Glfw::glThreadSafety() {
	std::thread::id current = std::this_thread::get_id();
	if (current != Glfw::thread_id) {
		D("Error: OpenGL call in wrong thread:\n\tcurrent:\t" << current << "\n\tOpenGL: \t" << Glfw::thread_id << "\n")
		exit(5);
	}
}

void	Glfw::initDefaultState() {
	D(__PRETTY_FUNCTION__ << "\n")
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MINOR);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

Glfw::Glfw() {
	D(__PRETTY_FUNCTION__ << "\n")
	this->_width = DEFAULT_WIDTH;
	this->_height = DEFAULT_HEIGHT;
	this->_title = DEFAULT_TITLE;
	this->init();
}

Glfw::Glfw(unsigned int width, unsigned int height) {
	D(__PRETTY_FUNCTION__ << "\n")
	this->_width = std::min(width, (unsigned int)MAX_WIDTH);
	this->_width = std::max(width, (unsigned int)MIN_WIDTH);
	this->_height = std::min(height, (unsigned int)MAX_HEIGHT);
	this->_height = std::max(height, (unsigned int)MIN_HEIGHT);
	this->_title = DEFAULT_TITLE;
	this->init();
}

Glfw::~Glfw() {
	D(__PRETTY_FUNCTION__ << "\n")
	glfwDestroyWindow(this->_window);
}

void	Glfw::printExtensions() const {
	GLint n = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	D("Glfw extensions : " << n << "\n")
	for (GLint i = 0; i < n; i++) {
		const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
		D(i << " :\t" << extension << "\n")
	}
}

void	Glfw::init() {
	D(__PRETTY_FUNCTION__ << "\n")

	this->_mouseX = 0;
	this->_mouseY = 0;
	this->_mouseOriginX = 0;
	this->_mouseOriginY = 0;
	this->_mouseDiffX = 0;
	this->_mouseDiffY = 0;
	this->_mouseOffsetX = 0;
	this->_mouseOffsetY = 0;
	this->_mouseAngle = 0;
	this->_mouseWall = 0;
	if (!glfwInit()) {
		D("glfwInit failed\n")
		Misc::breakExit(GL_ERROR);
	}
	glfwSetErrorCallback(defaultErrorCallback);
	Glfw::initDefaultState();
	this->_window = glfwCreateWindow(this->_width, this->_height, this->_title.c_str(), NULL, NULL);
	if (!this->_window) {
		D("glfwCreateWindow failed\n")
		Misc::breakExit(GL_ERROR);
	}
	glfwMakeContextCurrent(this->_window);//to do before init glew
	glfwSetWindowPos(this->_window, 0, 0);

	this->cursorFree = false;
	glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(this->_window, GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(this->_window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	//glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		D("glewInit failed\n")
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
	D("GL version: " << glGetString(GL_VERSION) << "\n")
}

void	Glfw::updateMouse() {
	glfwGetCursorPos(this->_window, &this->_mouseX, &this->_mouseY);
	
	if (false) {
		D(this->_mouseX << " : " << this->_mouseY << "\t(" << this->_mouseOriginX << " : " << this->_mouseOriginY << ")\n")
	} else if (false) {
		//diff
		D(this->_mouseOriginX - this->_mouseX << " : " << this->_mouseOriginY - this->_mouseY << "\n")
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

void	Glfw::toggleCursor() {
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

void	Glfw::activateDefaultCallbacks(GameManager * manager) {
	glfwSetWindowUserPointer(this->_window, manager);
	glfwSetWindowCloseCallback(this->_window, defaultWindowCloseCallback);
	glfwSetKeyCallback(this->_window, defaultkeyCallback);
	glfwSetMouseButtonCallback(this->_window, defaultMouseButtonCallback);
	
	// key map
	this->func[GLFW_KEY_TAB] = keyCallback_tab;
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
int			Glfw::getWidth() const { return (this->_width); }
int			Glfw::getHeight() const { return (this->_height); }

//shoudl never be called
Glfw&		Glfw::operator=(const Glfw& src) {
	D(__PRETTY_FUNCTION__ << "\n")
	D("WTF\n")
	exit(0);
	(void)src;
	return *this;
}
