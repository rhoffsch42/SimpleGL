#pragma once

#include "simplegl.h"
#include "cam.hpp"
#include "gamemanager.hpp"

#include <algorithm>
#include <map>
#include <thread>

#define DEFAULT_WIDTH	800
#define DEFAULT_HEIGHT	800
#define MIN_WIDTH		400
#define MIN_HEIGHT		400
#define MAX_WIDTH		1920 // a definir selon la resolution de l'ecran ?
#define MAX_HEIGHT		1080
#define MOUSE_MAX_ANGLE	90.0f //vertical angle

#define DEFAULT_TITLE	"Default Title"

//must be 4+
#define GL_MAJOR		4
#define GL_MINOR		1

/*
	fenetre comprise dans la classe pour le moment (1 fenetre max)
*/

class GameManager;

typedef	void	(*t_glfwKeyCallback)(GLFWwindow*, int, int, int, int);

class Glfw
{
#if UNIT_TESTS == true
	friend class UnitTests;
#endif
public:
	static std::thread::id	thread_id;
	static void	glThreadSafety();
	static void	initDefaultState();

	Glfw();
	Glfw(unsigned int width, unsigned int height);
	Glfw&	operator=(const Glfw& src);
	~Glfw();
	void			printExtensions() const;
	void			updateMouse();

	//use it to toggle the cursor. Care when using glfwSetInputMode, this can lead to conflicts
	void			toggleCursor();
	void			activateDefaultCallbacks(GameManager * manager);

	//mutator
	void			setMouseAngle(double angle);//degree, set to negative to deactivate
	void			setTitle(std::string newTitle);
	//accesor
	double			getMouseAngle() const;
	std::string		getTitle() const;
	int				getWidth() const;
	int				getHeight() const;

	GLFWwindow *	_window;//private?
	bool			cursorFree;
	std::map<int, t_glfwKeyCallback>	func;

private://changer double en uint ? cf glfwGetCursorPos
	unsigned int	_width;
	unsigned int	_height;
	std::string		_title;

	double			_mouseX;
	double			_mouseY;
	double			_mouseOriginX;
	double			_mouseOriginY;
	double			_mouseDiffX;
	double			_mouseDiffY;
	double			_mouseOffsetX;
	double			_mouseOffsetY;
	double			_mouseAngle;
	double			_mouseWall;

	void	init();

	friend class Cam;
};

