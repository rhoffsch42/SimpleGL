#pragma once
#include "simplegl.h"

class Blueprint
{
public:
	~Blueprint();
	Blueprint&	operator=(const Blueprint& bp);

	//accessors
	GLuint		getVao(void) const;
	std::string	getName(void) const;

protected:
	GLuint		_vao;
	std::string	_name;

	Blueprint(std::string filename);
	Blueprint(const Blueprint& src);
private:
};

