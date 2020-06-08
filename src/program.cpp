#include "program.hpp"
#include "compiler_settings.h"

Program::Program(std::string vs_file, std::string fs_file) {
	std::cout << "_ " << __PRETTY_FUNCTION__ << std::endl;

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
	std::cout << "Successfully created Program:: " << this->_program << std::endl;
}

Program::~Program() {
	std::cout << "_ " << __PRETTY_FUNCTION__ << std::endl;
}

//void	Program::render() {
//	cout << "Program::render()\nThis should not be used..." << std::endl;
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
	std::cout << "slot " << slot << " :\t" << varname << std::endl;
	if (slot == -1) {
		//int errs[8]= { GL_NO_ERROR, GL_INVALID_ENUM, GL_INVALID_VALUE, GL_INVALID_OPERATION, GL_INVALID_FRAMEBUFFER_OPERATION, GL_OUT_OF_MEMORY, GL_STACK_UNDERFLOW, GL_STACK_OVERFLOW };
		//size_t error = glGetError();
		std::cerr << "Failed to get slot" << std::endl;
		std::cerr << "program:\t" << this->_program << std::endl;
		std::cerr << "varname:\t" << varname << std::endl;
		exit(GL_ERROR);
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
	std::cerr << intro << std::endl << info << std::endl;
	std::cerr << GL_COMPILE_SHADER << std::endl;
	exit(GL_ERROR);
}

void	Program::printProgramInfoLog(GLuint program) const {
	int		params;
	int		max_len;
	int		len;
	char	logs[2048];

	params = -1;
	std::cerr << "ERROR: could not link shader program GL index " << program << std::endl;
	std::cerr << "--------------------" << std::endl << "shader program " << program << " info:" << std::endl;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	std::cerr << "GL_LINK_STATUS = " << params << std::endl;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
	std::cerr << "GL_ATTACHED_SHADERS = " << params << std::endl;
	this->programLogs(program, GL_ACTIVE_ATTRIBUTES, false, "GL_ACTIVE_ATTRIBUTES = ");
	this->programLogs(program, GL_ACTIVE_UNIFORMS, true, "GL_ACTIVE_UNIFORMS = ");
	max_len = 2048;
	len = 0;
	glGetProgramInfoLog(program, max_len, &len, logs);
	std::cerr << "program info log for GL index " << program << ": (" << len << " char)" << std::endl << logs << std::endl;
	// this->~Program();
	exit(GL_ERROR);
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
	std::cerr << msg.c_str() << params << std::endl;
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
