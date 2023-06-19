#include "simplegl.h"
#include "compiler_settings.h"
#include "blueprint.hpp"

#ifdef SGL_DEBUG
 //#define SGL_BLUEPRINT_DEBUG
#endif
#ifdef SGL_BLUEPRINT_DEBUG 
 #define D(x) std::cout << "[Blueprint] " << x ;
 #define D_(x) x
 #define D_SPACER "-- blueprint.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

Blueprint::Blueprint(std::string filename) {
	D("Blueprint cons by filename" << std::endl)
	D("building object: " << filename.c_str() << std::endl)
	this->_vao = 0;
	this->_name = filename;
}

Blueprint::Blueprint(const Blueprint& src) {
	D("Blueprint cons by copy" << std::endl)
	D("building object: " << src.getName().c_str() << std::endl)
	*this = src;
}

Blueprint::~Blueprint() {
	//D("Blueprint des" << std::endl)
}

Blueprint& Blueprint::operator=(const Blueprint& src) {
	D("Blueprint operator =" << std::endl)

	this->_vao = src.getVao();
	this->_name = src.getName();
	return (*this);
}

GLuint	Blueprint::createVao() {
	if (this->_vao != 0) {
		D("overriding the vao: " << this->_vao << ". This shouldn't happen.\n")
		Misc::breakExit(-56);
	}
	glGenVertexArrays(1, &this->_vao);
	D("creating vao: " << this->_vao << " for bp: " << this << "\n")
	return this->_vao;
}
/*
	a vao is linked to a context, we dont know the context it was originally created in
	how to know this context?
	this function used in the wrong context can lead to the deletion of the wrong vao
*/
void	Blueprint::deleteVao() {
	D("deleting vao: " << this->_vao << " for bp: " << this << "\n")
	glDeleteVertexArrays(1, &this->_vao);
	this->_vao = 0;
}

//accessors
GLuint		Blueprint::getVao(void) const { return (this->_vao); }
std::string	Blueprint::getName(void) const { return (this->_name); }
