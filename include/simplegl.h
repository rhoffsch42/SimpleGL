#pragma once

#define IGNORE_UNUSED_VARIABLES
#define IGNORE_WARNING_LIBS

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#ifdef IGNORE_UNUSED_VARIABLES
	#ifdef _WIN32 //ignore unused variables
	//windoes equivalent
	#else
	#pragma clang diagnostic push	//clang++
	#pragma clang diagnostic ignored "-Wunused-variable"
	#endif //ignore unused variables
#endif

#ifdef IGNORE_WARNING_LIBS
	#ifdef _WIN32
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
	#endif
#else
	#include <GL/glew.h>
	#include <GLFW/glfw3.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <sstream>

#include "misc.hpp"

#define EMOTE_OK			"\xe2\x9c\x85 " // ✅
#define EMOTE_FAIL			"\xe2\x9d\x8c " // ❌
#define EMOTE_WARNING		"\xe2\x9a\xa0\xef\xb8\x8f " // ⚠️

//#define SGL_DEBUG
#define LOGFILES	true

#define OBJ3D_VS_FILE			"shaders/obj3d.vs.glsl"
#define OBJ3D_FS_FILE			"shaders/obj3d.fs.glsl"
#define OBJ3D_INSTANCED_VS_FILE	"shaders/obj3d_instanced.vs.glsl"
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

//functions override
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
