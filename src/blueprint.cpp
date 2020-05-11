#include "simplegl.h"
#include "blueprint.hpp"

Blueprint::Blueprint(std::string filename) {
	cout << "_ Blueprint cons by filename" << endl;

	cout << "building object: " << filename.c_str() << endl;
	this->_vao = 0;
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
	this->_name = src.getName();
	return (*this);
}

//mutators
//accessors
GLuint		Blueprint::getVao(void) const { return (this->_vao); }
string		Blueprint::getName(void) const { return (this->_name); }
