#include "simplegl.h"
#include "blueprint.hpp"

Blueprint::Blueprint(std::string filename) {
	//cout << "_ Blueprint cons by filename" << std::endl;

	//cout << "building object: " << filename.c_str() << std::endl;
	this->_vao = 0;
	this->_name = filename;
}

Blueprint::Blueprint(const Blueprint& src) {
	std::cout << "_ Blueprint cons by copy" << std::endl;

	std::cout << "building object: " << src.getName().c_str() << std::endl;
	*this = src;
}

Blueprint::~Blueprint() {
	//std::cout << "_ Blueprint des" << std::endl;
	/*
		detruire les vbo/vao/etc d'openGL
	*/
}

Blueprint& Blueprint::operator=(const Blueprint& src) {
	std::cout << "_ Blueprint operator =" << std::endl;

	this->_vao = src.getVao();
	this->_name = src.getName();
	return (*this);
}

//mutators
//accessors
GLuint		Blueprint::getVao(void) const { return (this->_vao); }
std::string	Blueprint::getName(void) const { return (this->_name); }
