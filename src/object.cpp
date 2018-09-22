#include "simplegl.h"
#include "object.hpp"

//static variables initialisation
unsigned int	Object::getInstanceAmount() { return (Object::_instanceAmount); }
unsigned int	Object::_instanceAmount = 0;
unsigned int	Object::_instanceId = 0;

static float	calcScaleCoef(Math::Vector3 dimensions, float size) {
	float	largest = dimensions.x;
	largest = std::max(largest, dimensions.y);
	largest = std::max(largest, dimensions.z);
	return (size / largest);
}

Object::Object() {
	cout << "_ Object cons" << endl;
	this->_id = Object::_instanceId;
	this->_motionBehavior = false;
	this->_motionBehaviorFunc = NULL;
	this->_parent = NULL;

	Object::_instanceAmount++;
	Object::_instanceId++;
}

Object::Object(const Object& src) {
	cout << "_ Object cons by copy" << endl;

	*this = src;
}

Object&		Object::operator=(const Object& src) {
	this->_motionBehavior = src._motionBehavior;
	this->_motionBehaviorFunc = src.getMotionBehaviorFunc();

	this->_id = Object::_instanceId;
	this->_world = src.getWorldProperties();
	this->_local = src.getLocalProperties();
	// x+1 y+1 z+1 pour faire un decalage // tmp
	this->_local.translate(1, 1, 1);

	Object::_instanceAmount++;
	Object::_instanceId++;
	return (*this);
}

Object::~Object() {
	cout << "_ Object des" << endl;
	Object::_instanceAmount--;
}

void		Object::runMothionBehavior(void * ptr) {
	if (this->_motionBehaviorFunc && this->_motionBehavior)
		this->_motionBehaviorFunc(*this, ptr);
	else
		cout << "No motion behavior is set/activated. Doing nothing." << endl;
}

void		Object::render(Math::Matrix4& PVmatrix) {
	if (!this->_matrixUpdated) {
		Math::Vector3 &	usedScale = this->_rescaled ? this->_finalScale : this->_scale;
		this->_modelMatrix.modelMatrix(this->_pos, this->_rot, usedScale);
		if (this->centered)
			this->center();
		this->_matrixUpdated = true;
	}
	this->_program.render((Object&)(*this), PVmatrix);
}

//mutators
//accessors
unsigned int	Object::getId(void) const { return (this->_id); }
void			(*Object::getMotionBehaviorFunc(void) const) (Object &, void*) { return (this->_motionBehaviorFunc); }
Properties		Object::getWorldProperties() const { return (this->_world); }
Properties		Object::getLocalProperties() const { return (this->_local); }
