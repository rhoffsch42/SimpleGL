#include "simplegl.h"
#include "program.hpp"
#include "compiler_settings.h"

#ifdef SGL_DEBUG
 #define SGL_PROGRAM_DEBUG
#endif
#ifdef SGL_PROGRAM_DEBUG 
 #define D(x) std::cout << "[Program] " << x ;
 #define D_(x) x
 #define D_SPACER "-- program.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

Program::Program(std::string vs_file, std::string fs_file) {
	D(__PRETTY_FUNCTION__ << std::endl)

	vs_file = Misc::crossPlatPath(vs_file);
	fs_file = Misc::crossPlatPath(fs_file);
	GLuint	vertexShader = initShader(vs_file, GL_VERTEX_SHADER);
	GLuint	fragmentShader = initShader(fs_file, GL_FRAGMENT_SHADER);
	this->_program = glCreateProgram();
	glAttachShader(this->_program, vertexShader);
	glAttachShader(this->_program, fragmentShader);
	glLinkProgram(this->_program);
	int	p = -1;
	glValidateProgram(this->_program);
	glGetProgramiv(this->_program, GL_LINK_STATUS, &p);
	if (p != GL_TRUE)
		this->printProgramInfoLog(this->_program);
	//shader objects are useless once the program is linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	D("Successfully created Program:: " << this->_program << std::endl)
}

Program::~Program() {
	D(__PRETTY_FUNCTION__ << std::endl)
	glDeleteProgram(this->_program);
}

//void	Program::render() {
//	D("Program::render()\nThis should not be used..." << std::endl)
//}

/*
	true	glGetUniformLocation
	false	glGetAttribLocation
*/
GLint	Program::getSlot(const GLchar* varname, bool n) const {
	GLint	slot;

	if (n)
		slot = glGetUniformLocation(this->_program, varname);
	else
		slot = glGetAttribLocation(this->_program, varname);
	D("slot " << slot << " :\t" << varname << std::endl)
	if (slot == -1) {
		//int errs[8]= { GL_NO_ERROR, GL_INVALID_ENUM, GL_INVALID_VALUE, GL_INVALID_OPERATION, GL_INVALID_FRAMEBUFFER_OPERATION, GL_OUT_OF_MEMORY, GL_STACK_UNDERFLOW, GL_STACK_OVERFLOW };
		//size_t error = glGetError();
		D("Failed to get slot" << std::endl)
		D("program:\t" << this->_program << std::endl)
		D("varname:\t" << varname << std::endl)
		Misc::breakExit(GL_ERROR);
	}
	return (slot);
}

GLuint	Program::initShader(std::string filename, int type) const {
	GLint			ret = GL_FALSE;
	GLuint			shader;
	const GLchar* gl_content;
	std::string		buf;

	buf = Misc::getFileContent(filename.c_str());
	gl_content = buf.c_str();
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &gl_content, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);
	if (ret == GL_FALSE)
		glCompileError(shader, "shader compilation error:");
	return (shader);
}

void	Program::glCompileError(GLuint shader, const char* intro) const {
	GLsizei		maxl;
	GLsizei		l;
	GLchar* info;

	maxl = 1000;
	info = (GLchar*)malloc(1000);
	glGetShaderInfoLog(shader, maxl, &l, info);
	D(intro << std::endl << info << std::endl)
	D(GL_COMPILE_SHADER << std::endl)
	Misc::breakExit(GL_ERROR);
}

void	Program::printProgramInfoLog(GLuint program) const {
	int		params;
	int		max_len;
	int		len;
	char	logs[2048];

	params = -1;
	D("ERROR: could not link shader program GL index " << program << std::endl)
	D("--------------------" << std::endl << "shader program " << program << " info:" << std::endl)
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	D("GL_LINK_STATUS = " << params << std::endl)
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
	D("GL_ATTACHED_SHADERS = " << params << std::endl)
	this->programLogs(program, GL_ACTIVE_ATTRIBUTES, false, "GL_ACTIVE_ATTRIBUTES = ");
	this->programLogs(program, GL_ACTIVE_UNIFORMS, true, "GL_ACTIVE_UNIFORMS = ");
	max_len = 2048;
	len = 0;
	glGetProgramInfoLog(program, max_len, &len, logs);
	D("program info log for GL index " << program << ": (" << len << " char)" << std::endl << logs << std::endl)
	// this->~Program();
	Misc::breakExit(GL_ERROR);
}

/*
	true	glGetUniformLocation
	false	glGetAttribLocation
*/

void	Program::programLogs(GLuint program, GLenum pname, bool n, std::string msg) const {
	int		params = -1;
	char	name[64];
	char	long_name[64];
	int		length = 64;
	int		actual_length = 64;
	int		size = 0;
	int		location;
	GLenum	type;
	int		i = -1;
	int		j;

	glGetProgramiv(program, pname, &params);
	D(msg.c_str() << params << std::endl)
	while (++i < params)
	{
		glGetActiveAttrib(program, i, length, &actual_length, &size, &type, name);
		j = -1;
		if (size > 1)
			while (++j < size)
			{
				sprintf(long_name, "%s[%i]", name, j);
				location = n ? glGetUniformLocation(program, long_name) : glGetAttribLocation(program, long_name);
				fprintf(stderr, " %i) type:%s name:%s location:%i\n", i, this->glTypeToString(type), long_name, location);
			}
		else
		{
			location = n ? glGetUniformLocation(program, long_name) : glGetAttribLocation(program, long_name);
			fprintf(stderr, " %i) type:%s name:%s location:%i\n", i, this->glTypeToString(type), name, location);
		}
	}
}

const char*	Program::glTypeToString(GLenum type) const { // refacto en map type:char_value
	if (type == GL_BOOL)
		return ("bool");
	if (type == GL_INT)
		return ("int");
	if (type == GL_FLOAT)
		return ("float");
	if (type == GL_FLOAT_VEC2)
		return ("vec2");
	if (type == GL_FLOAT_VEC3)
		return ("vec3");
	if (type == GL_FLOAT_VEC4)
		return ("vec4");
	if (type == GL_FLOAT_MAT2)
		return ("mat2");
	if (type == GL_FLOAT_MAT3)
		return ("mat3");
	if (type == GL_FLOAT_MAT4)
		return ("mat4");
	if (type == GL_SAMPLER_2D)
		return ("sampler2D");
	if (type == GL_SAMPLER_3D)
		return ("sampler3D");
	if (type == GL_SAMPLER_CUBE)
		return ("samplerCube");
	if (type == GL_SAMPLER_2D_SHADOW)
		return ("sampler2DShadow");
	return ("other");
}
