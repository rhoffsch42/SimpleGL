#include "simplegl.h"
#include "blueprint.hpp"

Blueprint::Blueprint(std::string filename) {
	cout << "_ Blueprint cons by filename" << endl;

	cout << "building object: " << filename.c_str() << endl;
	this->_vao = 0;
	this->_vboVertex = 0;
	this->_vboColor = 0;
	this->_vboTexture = 0;
	this->_name = filename;
}

Blueprint::Blueprint(const Blueprint& src) {
	cout << "_ Blueprint cons by copy" << endl;

	cout << "building object: " << src.getName().c_str() << endl;
	*this = src;
}

Blueprint::~Blueprint() {
	cout << "_ Blueprint des" << endl;
	/*
		detruire les vbo/vao/etc d'openGL
	*/
}

Blueprint&		Blueprint::operator=(const Blueprint& src) {
	cout << "_ Blueprint operator =" << endl;

	this->_vao = src.getVao();
	this->_vboVertex = src.getVboVertex();
	this->_vboTexture = src.getVboTexture();
	this->_name = src.getName();
	return (*this);
}

//mutators
//accessors
GLuint&		Blueprint::getVao(void) const { return ((GLuint&)this->_vao); }
GLuint&		Blueprint::getVboVertex(void) const { return ((GLuint&)this->_vboVertex); }
GLuint&		Blueprint::getVboColor(void) const { return ((GLuint&)this->_vboColor); }
GLuint&		Blueprint::getVboTexture(void) const { return ((GLuint&)this->_vboTexture); }
string		Blueprint::getName(void) const { return (this->_name); }
