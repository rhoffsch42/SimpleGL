#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "object.hpp"
#include "cam.hpp"

#include <iostream>
#include <cstdio>
using namespace std;

#define GL_COMPILE_SHADER		"OpenGL shader error"

class Program
{
public:
	Program(std::string vs_file, std::string fs_file);
	~Program();
	//tmp public var
	GLuint		_program;
protected:
	GLint			getSlot(const GLchar* varname, bool n) const;

	//a definir dans classe fille, ou faire un lecteur de shader et mapper les slots
	virtual void	getLocations() = 0;
	virtual void	render(Object& object, Math::Matrix4 VPmatrix) const = 0;
	virtual void	renderObjects(list<Object*>& list, Cam& cam, bool force_draw = false) = 0;//const?
private:
	GLuint			_vertex_shader;
	GLuint			_fragment_shader;

	// utils
	GLuint			initShader(std::string filename, int type) const;
	void			glCompileError(GLuint shader, const char *intro) const;
	void			printProgramInfoLog(GLuint program) const;
	void			programLogs(GLuint program, GLenum pname, bool n, std::string msg) const;
	const char*		glTypeToString(GLenum type) const;
};
