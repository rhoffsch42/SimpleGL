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

class Glfw
{
public:
	Glfw();
	Glfw(unsigned int width, unsigned int height);
	~Glfw();
	void		updateMouse();
	//mutator
	void		setMouseAngle(double angle);//degree, set to negative to deactivate
	void		setTitle(std::string newTitle);
	//accesor
	double		getMouseAngle() const;
	std::string	getTitle() const;

	GLFWwindow *	_window;//private?

private://changer double en uint ? cf glfwGetCursorPos
	unsigned int	_width;
	unsigned int	_height;
	std::string		_title;
	double			_mouseOriginX;
	double			_mouseOriginY;
	double			_mouseAngle;
	double			_mouseWall;
	double			_mouseX;
	double			_mouseY;
	double			_mouseDiffX;
	double			_mouseDiffY;

	void	init();

	friend class Cam;
};

