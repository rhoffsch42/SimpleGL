#pragma once

#include "simplegl.h"
#include "lod_manager.hpp"

class Blueprint
{
public:
	virtual ~Blueprint();
	Blueprint&	operator=(const Blueprint& bp);

	GLuint	createVao();
	void	deleteVao();

	//accessors
	GLuint		getVao(void) const;
	std::string	getName(void) const;

	LodManager	lodManager;
protected:
	GLuint		_vao = 0;
	std::string	_name;

	Blueprint(std::string filename);
	Blueprint(const Blueprint& src);
private:
};

