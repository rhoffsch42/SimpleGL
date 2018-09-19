#include "simplegl.h"
#include "obj3d.hpp"

//static variables initialisation
unsigned int	Obj3d::getInstanceAmount() { return (Obj3d::instanceAmount); }
unsigned int	Obj3d::instanceAmount = 0;
unsigned int	Obj3d::instanceId = 0;
bool			Obj3d::defaultCentered = false;
bool			Obj3d::defaultRotate = false;
bool			Obj3d::defaultDisplayTexture = false;
GLenum			Obj3d::defaultPolygonMode = GL_FILL;
float			Obj3d::defaultSize = OBJ3D_DEFAULT_SIZE;
bool			Obj3d::defaultRescaled = true;

static float	calcScaleCoef(Math::Vector3 dimensions, float size) {
	float	largest = dimensions.x;
	largest = std::max(largest, dimensions.y);
	largest = std::max(largest, dimensions.z);
	return (size / largest);
}

Obj3d::Obj3d(Obj3dBP& bp, Obj3dPG& pg) \
: _blueprint(bp), _program(pg), _scaleCoef(calcScaleCoef(bp.getDimensions(), Obj3d::defaultSize)) {
	cout << "_ Obj3d cons" << endl;
	this->_id = Obj3d::instanceId;
	this->_scale = Math::Vector3(1.0f, 1.0f, 1.0f);
	this->_finalScale = this->_scale;
	this->_finalScale.mult(this->_scaleCoef);
	this->_rescaled = Obj3d::defaultRescaled;
	Math::Vector3 &	usedScale = this->_rescaled ? this->_finalScale : this->_scale;
	this->_modelMatrix.modelMatrix(this->_pos, this->_rot, usedScale);
	this->_matrixUpdated = true;
	this->_texture = NULL;
	this->_polygonMode = Obj3d::defaultPolygonMode;
	this->_displayTexture = Obj3d::defaultDisplayTexture;
	this->_rotate = Obj3d::defaultRotate;
	this->_centered = Obj3d::defaultCentered;

	GLuint&		vboVertex = this->_blueprint.getVboVertex();
	GLuint&		vboColor = this->_blueprint.getVboColor();
	GLuint&		vboTexture = this->_blueprint.getVboTexture();
	glBindVertexArray(this->_blueprint.getVao());//bind the VAO to be able to link buffers to the program
	this->_program.linkBuffers(vboVertex, vboColor, vboTexture);
	glBindVertexArray(0);//necessaire ? utile ? propre ?

	Obj3d::instanceAmount++;
	Obj3d::instanceId++;
}

Obj3d::Obj3d(const Obj3d& src) \
: _blueprint(src.getBlueprint()), _program(src.getProgram()), _scaleCoef(src.getScaleCoef()) {
	cout << "_ Obj3d cons by copy" << endl;

	*this = src;
}

Obj3d&		Obj3d::operator=(const Obj3d& src) {
	this->_displayTexture = src._displayTexture;
	this->_rotate = src._rotate;
	this->_centered = src._centered;
	this->_rescaled = src._rescaled;

	this->_id = Obj3d::instanceId;
	this->_blueprint = src.getBlueprint();
	this->_program = src.getProgram();
	this->_modelMatrix = src.getModelMatrix();
	this->_matrixUpdated = false;
	// x+1 y+1 z+1 pour faire un decalage // tmp
	Math::Vector3	pos = src.getPos();
	pos.add(Math::Vector3(1, 1, 1));
	this->setPos(pos.x, pos.y, pos.z);
	this->_scale = src.getScale();
	this->setRot(src.getRot());
	this->_color = src.getColor();
	this->_texture = src.getTexture();
	this->_polygonMode = src.getPolygonMode();

	if (this->_centered)
		this->center();

	Obj3d::instanceAmount++;
	Obj3d::instanceId++;
	return (*this);
}

Obj3d::~Obj3d() {
	cout << "_ Obj3d des" << endl;
	Obj3d::instanceAmount--;
}

void		Obj3d::render(Math::Matrix4& PVmatrix) {
	if (!this->_matrixUpdated) {
		Math::Vector3 &	usedScale = this->_rescaled ? this->_finalScale : this->_scale;
		this->_modelMatrix.modelMatrix(this->_pos, this->_rot, usedScale);
		if (this->_centered)
			this->center();
		this->_matrixUpdated = true;
	}
	this->_program.render((Obj3d&)(*this), PVmatrix);
}

void		Obj3d::center() {
	//a faire a chaque changement de pos/rot/scale
/*
	https://gamedev.stackexchange.com/questions/59843/rotating-an-object-when-the-center-in-not-the-origin-opengl
	Easy way of building the rotation matrix :
		1	Start with an identity matrix
		2	Translate the matrix by - centre of the object
		3	Rotate the matrix by the desired amount
		4	Translate the matrix by centre of the object
		5	Use the resulting matrix to transform the object that you desire to rotate
*/
	Math::Vector3	newpos = this->_pos;
	Math::Vector3	offset = this->_blueprint.getCenterOffset();
	Math::Vector3 &	usedScale = this->_rescaled ? this->_finalScale : this->_scale;
	offset.x *= usedScale.x;
	offset.y *= usedScale.y;
	offset.z *= usedScale.z;
	Math::Vector3	offsetneg(-offset.x, -offset.y, -offset.z);
	offsetneg.rotate(this->_rot, ROT_WAY);
	newpos.add(offsetneg);
	this->_modelMatrix.updatePosValue(newpos);
}

//mutators
void		Obj3d::setPos(float x, float y, float z) {
	this->_pos.x = x;
	this->_pos.y = y;
	this->_pos.z = z;
	if (this->_centered)
		this->center();
	else
		this->_modelMatrix.updatePosValue(this->_pos);
}
void		Obj3d::setPos(Math::Vector3 pos) {
	this->_pos = pos;
	if (this->_centered)
		this->center();
	else
		this->_modelMatrix.updatePosValue(this->_pos);
}
void		Obj3d::setRot(float x, float y, float z) {// in degree!
	this->_rot.x = x;
	this->_rot.y = y;
	this->_rot.z = z;
	this->_rot.setUnit(ROT_DEG);
	this->_matrixUpdated = false;
}
void		Obj3d::setRot(Math::Rotation rot) {
	this->_rot = rot;
	this->_matrixUpdated = false;
}
void		Obj3d::setScale(float x, float y, float z) {
	this->_scale.x = x;
	this->_scale.y = y;
	this->_scale.z = z;
	this->_finalScale = this->_scale;
	this->_finalScale.mult(this->_scaleCoef);
	this->_matrixUpdated = false;
}
void		Obj3d::setScale(Math::Vector3 scale) {
	this->_scale = scale;
	this->_finalScale = this->_scale;
	this->_finalScale.mult(this->_scaleCoef);
	this->_matrixUpdated = false;
}
void		Obj3d::setRescaled(bool value) {
	this->_rescaled = value;
	this->_matrixUpdated = false;
}
void		Obj3d::setColor(uint8_t x, uint8_t y, uint8_t z) {
	this->_color.x = x;
	this->_color.y = y;
	this->_color.z = z;
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
unsigned int	Obj3d::getId(void) const { return (this->_id); }
Obj3dBP&		Obj3d::getBlueprint(void) const { return (this->_blueprint); }
Obj3dPG&		Obj3d::getProgram(void) const { return (this->_program); }
Math::Matrix4&	Obj3d::getModelMatrix(void) const { return ((Math::Matrix4&)this->_modelMatrix); }
Math::Vector3	Obj3d::getPos(void) const { return (this->_pos); }
Math::Rotation	Obj3d::getRot(void) const { return (this->_rot); }
Math::Vector3	Obj3d::getScale(void) const { return (this->_scale); }
Math::Vector3	Obj3d::getFinalScale(void) const { return (this->_finalScale); }
float			Obj3d::getScaleCoef(void) const { return (this->_scaleCoef); }
bool			Obj3d::isRescaled() const { return (this->_rescaled); }
Math::Vector3	Obj3d::getColor(void) const { return (this->_color); }
Texture*		Obj3d::getTexture(void) const { return (this->_texture); }
GLenum			Obj3d::getPolygonMode() const { return (this->_polygonMode); }
