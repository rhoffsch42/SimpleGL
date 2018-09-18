//  > output.txt 2>&1
#pragma once


#ifdef _WIN32 //ignore unused variables
//windoes equivalent
#else
#pragma clang diagnostic push	//clang++
#pragma clang diagnostic ignored "-Wunused-variable"
#endif //ignore unused variables

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

#ifdef _WIN32
#include <Windows.h>
#endif
#include <stdio.h>
#include <iostream>
#include <sstream>
using namespace std;
#include "misc.hpp"

#define LOGFILES	true

#define OBJ3D_VS_FILE			"shaders/obj3d.vs.glsl"
#define OBJ3D_FS_FILE			"shaders/obj3d.fs.glsl"
#define CUBEMAP_VS_FILE			"shaders/cubemap.vs.glsl"
#define CUBEMAP_FS_FILE			"shaders/cubemap.fs.glsl"
#define OBJ_CUBE_TEST			"obj3d/cube.obj"

#define OK				0
#define UNKNOW_ERROR	1
#define GL_ERROR		101
#define ERROR_PADDING	102

#define RAD_DELTA		1.0f
#define POS_DELTA		2.5f

#define FPS					60
#define MAX_FPS				120
#define MOUSE_SENSIBILITY	2000.0f
#define ROT_X				1

//functions overwrite
#if defined(__APPLE__) || defined(__linux__) 
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
