//  > output.txt 2>&1
#pragma once

#pragma clang diagnostic push	//clang++
#pragma clang diagnostic ignored "-Wunused-variable"

//#pragma warning(push, 0)		//visual studio
#pragma clang diagnostic push	//clang++
#pragma clang diagnostic ignored "-Wall"

#include <stdio.h>
//#include <tchar.h> //libpng, widows ?
//#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

// # include "humangl_opengl.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#pragma clang diagnostic pop	//clang++
//#pragma warning(pop)			//visual studio

#include "misc.hpp"

#define LOGFILES	true
//#define MATRIX_OPTI

#define OBJ3D_VS_FILE			"shaders/obj3d.vs.glsl"
#define OBJ3D_FS_FILE			"shaders/obj3d.fs.glsl"
#define CUBEMAP_VS_FILE			"shaders/cubemap.vs.glsl"
#define CUBEMAP_FS_FILE			"shaders/cubemap.fs.glsl"
#define OBJ_CUBE_TEST			"obj3d/cube.obj"

#define OK				0
#define UNKNOW_ERROR	1
#define GL_ERROR		101

#define RAD_DELTA		1.0f
#define POS_DELTA		2.5f

#define FPS					60
#define MAX_FPS				120
#define MOUSE_SENSIBILITY	2000.0f
#define ROT_X				1

typedef struct		s_glfw {
	int			size[2];
	std::string	title;
	GLFWwindow	*win;
	double		mouseX;
	double		mouseY;
	double		mouseOriginX;
	double		mouseOriginY;
	double		mouseWall;
}					t_glfw;

#ifdef __linux__ 
#define fopen		std::fopen
#define change_cwd	chdir
#elif __APPLE__
#define fopen		std::fopen
#define change_cwd	chdir
#elif _WIN32
#define getcwd		_getcwd
#define strdup		_strdup
#define fopen		windows_fopen_s
#define change_cwd	_chdir
#define sprintf		sprintf_s
#else
#error "OS not supported"
#endif
