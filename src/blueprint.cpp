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
}

Blueprint& Blueprint::operator=(const Blueprint& src) {
	std::cout << "_ Blueprint operator =" << std::endl;

	this->_vao = src.getVao();
	this->_name = src.getName();
	return (*this);
}

GLuint	Blueprint::createVao() {
	if (this->_vao != 0) {
		std::cerr << "overriding the vao: " << this->_vao << ". This shouldn't happen.\n";
		std::exit(-56);
	}
	glGenVertexArrays(1, &this->_vao);
	std::cout << "creating vao: " << this->_vao << " for bp: " << this << "\n";
	return this->_vao;
}
/*
	a vao is linked to a context, we dont know the context it was originally created in
	how to know this context?
	this function used in the wrong context can lead to the deletion of the wrong vao
*/
void	Blueprint::deleteVao() {
	std::cout << "deleting vao: " << this->_vao << " for bp: " << this << "\n";
	glDeleteVertexArrays(1, &this->_vao);
	this->_vao = 0;
}

//accessors
GLuint		Blueprint::getVao(void) const { return (this->_vao); }
std::string	Blueprint::getName(void) const { return (this->_name); }
