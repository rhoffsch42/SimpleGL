#pragma once

#include "simplegl.h"
//should include GL headers, no?

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
	virtual void	render();
	GLint			getSlot(const GLchar *varname, GLint(getLocFunc)(GLuint, const GLchar*)) const;

//	GLuint			_program;
private:
	GLuint			_vertex_shader;
	GLuint			_fragment_shader;

	//a definir dans classe fille
	virtual void	getLocations() = 0;
	
	// utils
	GLuint			initShader(std::string filename, int type) const;
	void			glCompileError(GLuint shader, const char *intro) const;
	void			printProgramInfoLog(GLuint program) const;
	void			programLogs(GLuint program, GLenum pname, GLint(getVarLocation)(GLuint, const GLchar *), std::string msg) const;
	const char		*glTypeToString(GLenum type) const;
};
