#include "simplegl.h"
#include "obj3d.hpp"

//static variables initialisation
unsigned int	Obj3d::getInstanceAmount() { return (Obj3d::_instanceAmount); }
unsigned int	Obj3d::_instanceAmount = 0;
bool			Obj3d::defaultDisplayTexture = false;
GLenum			Obj3d::defaultPolygonMode = GL_FILL;
Math::Vector3	Obj3d::defaultColor = OBJ3D_DEFAULT_COLOR;

Obj3d::Obj3d(Obj3dBP& bp, Obj3dPG& pg) : _blueprint(bp), _program(pg) {
	cout << "_ Obj3d cons" << endl;
	this->_color = Obj3d::defaultColor;
	this->_texture = nullptr;
	this->_polygonMode = Obj3d::defaultPolygonMode;
	this->displayTexture = Obj3d::defaultDisplayTexture;

	GLuint&		vboVertex = this->_blueprint.getVboVertex();
	GLuint&		vboColor = this->_blueprint.getVboColor();
	GLuint&		vboTexture = this->_blueprint.getVboTexture();
	glBindVertexArray(this->_blueprint.getVao());//bind the VAO to be able to link buffers to the program
	this->_program.linkBuffers(vboVertex, vboColor, vboTexture);
	glBindVertexArray(0);//necessaire ? utile ? propre ?

	Obj3d::_instanceAmount++;
}

Obj3d::Obj3d(const Obj3d& src) : Object(src), _blueprint(src.getBlueprint()), _program(src.getProgram()) {
	cout << "_ Obj3d cons by copy" << endl;
	*this = src;
}

Obj3d&		Obj3d::operator=(const Obj3d& src) {//doesnt call Object operator= ?
	this->displayTexture = src.displayTexture;
	this->_blueprint = src.getBlueprint();
	this->_program = src.getProgram();
	this->_color = src.getColor();
	this->_texture = src.getTexture();
	this->_polygonMode = src.getPolygonMode();

	Obj3d::_instanceAmount++;
	return (*this);
}

Obj3d::~Obj3d() {
	cout << "_ Obj3d des" << endl;
	Obj3d::_instanceAmount--;
	//Object destructor shouldbe called, check it
}

void		Obj3d::render(Math::Matrix4& PVmatrix) {
	this->update();
	this->_program.render((Obj3d&)(*this), PVmatrix);
	this->local._matrixChanged = false;
	this->_worldMatrixChanged = false;
}

//mutators
void		Obj3d::setColor(uint8_t x, uint8_t y, uint8_t z) {
	this->_color.x = x;
	this->_color.y = y;
	this->_color.z = z;

	this->_colorShader.x = x / 255.0f;
	this->_colorShader.y = y / 255.0f;
	this->_colorShader.z = z / 255.0f;
}
void		Obj3d::setTexture(Texture* texture) {
	this->_texture = texture;
}
void		Obj3d::setPolygonMode(GLenum mode) {
	/*
		GL_POINT	0x1B00
		GL_LINE		0x1B01
		GL_FILL		0x1B02
	*/
	if (mode >= GL_POINT && mode <= GL_FILL)
		this->_polygonMode = mode;
}

//accessors
Obj3dBP&		Obj3d::getBlueprint(void) const { return (this->_blueprint); }
Obj3dPG&		Obj3d::getProgram(void) const { return (this->_program); }
Math::Vector3	Obj3d::getColor(void) const { return (this->_color); }
Math::Vector3	Obj3d::getColorShader(void) const { return (this->_colorShader); }
Texture*		Obj3d::getTexture(void) const { return (this->_texture); }
GLenum			Obj3d::getPolygonMode(void) const { return (this->_polygonMode); }
