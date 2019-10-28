#include "simplegl.h"
#include "properties.hpp"

bool			Properties::defaultCentered = false;

Properties::Properties() {
	// cout << "_ Properties cons" << endl;
	this->_scale = Math::Vector3(1, 1, 1);
	this->_matrixUpdated = false;
	this->_matrixChanged = true;
}

Properties::Properties(const Properties& src) {
	// cout << "_ Properties cons by copy" << endl;
	*this = src;
}

Properties&		Properties::operator=(const Properties& src) {
	this->centered = src.centered;
	this->_matrix = Math::Matrix4(src.getMatrix());
	this->_matrixUpdated = false;
	this->_matrixChanged = true;
	this->_pos = src.getPos();
	this->_rot = src.getRot();
	this->_scale = src.getScale();
	this->_centerOffset = src.getCenterOffset();
	this->_centeredPos = src.getCenteredPos();
	return (*this);
}

Properties::~Properties() {
	// cout << "_ Properties des" << endl;
}

bool		Properties::updateMatrix() {
	// cout << "* Properties::updateMatrix" << endl;
	if (this->_matrixUpdated == false) {
		this->_matrix.modelMatrix(this->_pos, this->_rot, this->_scale);
		this->_matrixUpdated = true;
		this->_matrixChanged = true;
		return (false);
	}
	return (true);
}

/*
	Math::Vector3::RotateAround(...)
	https://gamedev.stackexchange.com/questions/59843/rotating-an-object-when-the-center-in-not-the-origin-opengl
	Easy way of building the rotation matrix :
		1	Start with an identity matrix
		2	Translate the matrix by - centre of the object
		3	Rotate the matrix by the desired amount
		4	Translate the matrix by centre of the object
		5	Use the resulting matrix to transform the object that you desire to rotate
*/

//relative mutators
void		Properties::translate(float x, float y, float z) {
	// this->_pos.operation(x, y, z);//cf .hpp
	this->_pos.x += x;
	this->_pos.y += y;
	this->_pos.z += z;
	this->_matrix.updatePosValue(this->_pos);
	this->_matrixChanged = true;
}
void		Properties::translate(Math::Vector3 pos) {
	// this->_pos.operation(Math::Vector3);//cf .hpp
	this->_pos.add(pos);
	this->_matrix.updatePosValue(this->_pos);
	this->_matrixChanged = true;
}
void		Properties::rotate(float x, float y, float z) {// in degree!
	// this->_rot.operation(x, y, z);//cf .hpp
	if (this->_rot.isRadian()) {
		x = Math::toRadian(x);
		y = Math::toRadian(y);
		z = Math::toRadian(z);
	}
	this->_rot.x += x;
	this->_rot.y += y;
	this->_rot.z += z;
	this->_matrixUpdated = false;
}
void		Properties::rotate(Math::Rotation rot) {
	// cout << "* Properties::rotate" << endl;
	// this->_rot.operation(Math::Vector3);//cf .hpp
	//todo: restore inital rotation unit ?
	if (rot.isRadian() != this->_rot.isRadian()) {
		rot.setAsRad();
		this->_rot.setAsRad();
	}
	this->_rot.add(rot);
	this->_matrixUpdated = false;
}
void		Properties::enlarge(float x, float y, float z) {
	// this->_scale.operation(x, y, z);//cf .hpp
	this->_scale.x += x;
	this->_scale.y += y;
	this->_scale.z += z;
	this->_matrixUpdated = false;
}
void		Properties::enlarge(Math::Vector3 scale) {
	// this->_scale.operation(Math::Vector3);//cf .hpp
	this->_scale.add(scale);
	this->_matrixUpdated = false;
}
//mutators
void		Properties::setPos(float x, float y, float z) {
	this->_pos.x = x;
	this->_pos.y = y;
	this->_pos.z = z;
	this->_matrix.updatePosValue(this->_pos);
	this->_matrixChanged = true;
}
void		Properties::setPos(Math::Vector3 pos) {
	this->_pos = pos;
	this->_matrix.updatePosValue(this->_pos);
	this->_matrixChanged = true;
}
void		Properties::setRot(float x, float y, float z) {// in degree!
	this->_rot.x = x;
	this->_rot.y = y;
	this->_rot.z = z;
	this->_rot.setUnit(ROT_DEG);
	this->_matrixUpdated = false;
}
void		Properties::setRot(Math::Rotation rot) {
	this->_rot = rot;
	this->_matrixUpdated = false;
}
void		Properties::setScale(float x, float y, float z) {
	this->_scale.x = x;
	this->_scale.y = y;
	this->_scale.z = z;
	this->_matrixUpdated = false;
}
void		Properties::setScale(Math::Vector3 scale) {
	this->_scale = scale;
	this->_matrixUpdated = false;
}

//accessors
Math::Matrix4&	Properties::getMatrix(void) const { return ((Math::Matrix4&)this->_matrix); }
Math::Vector3	Properties::getPos(void) const { return (this->_pos); }
Math::Rotation	Properties::getRot(void) const { return (this->_rot); }
Math::Vector3	Properties::getScale(void) const { return (this->_scale); }
Math::Vector3	Properties::getCenterOffset(void) const { return (this->_centerOffset); }
Math::Vector3	Properties::getCenteredPos(void) const { return (this->_centeredPos); }
