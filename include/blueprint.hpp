#pragma once
#include "simplegl.h"

class Blueprint
{
public:
	Blueprint(string filename);//temporary, still ?
	Blueprint(Blueprint const & src);
	~Blueprint();
	Blueprint&	operator=(const Blueprint& bp);

	//accessors
	GLuint&		getVao(void) const;
	GLuint&		getVboVertex(void) const;
	GLuint&		getVboColor(void) const;
	GLuint&		getVboTexture(void) const;
	string		getName(void) const;

protected:
	GLuint		_vao;
	GLuint		_vboVertex;
	GLuint		_vboColor;
	GLuint		_vboTexture;

	// file data
	string		_name;

private:
};

