#pragma once

#ifdef _WIN32 //external libraries
#pragma warning(push, 0)		//visual studio
#else
#pragma clang diagnostic push	//clang++
#pragma clang diagnostic ignored "-Wall"
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#pragma warning(pop)			//visual studio
#else
#pragma clang diagnostic pop	//clang++
#endif //external libraries

#include "simplegl.h"
#include "cam.hpp"

#ifdef _WIN32
#undef min
#undef max
#endif
#include <algorithm>
#include <map>

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

typedef	void	(*t_glfwKeyCallback)(GLFWwindow*, int, int, int, int);

class Glfw
{
#if UNIT_TESTS == true
	friend class UnitTests;
#endif
public:
	Glfw();
	Glfw(unsigned int width, unsigned int height);
	~Glfw();
	void			updateMouse();

	//use it to toggle the cursor. Care when using glfwSetInputMode, this can lead to conflicts
	void			toggleCursor();

	//mutator
	void			setMouseAngle(double angle);//degree, set to negative to deactivate
	void			setTitle(std::string newTitle);
	//accesor
	double			getMouseAngle() const;
	std::string		getTitle() const;


	// virtual void		onKey(int key, int scancode, int action, int mods);

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

