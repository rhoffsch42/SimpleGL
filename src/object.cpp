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

void		Object::update() {
	/* plus rapide mais incomplet:
		*si parent:
			world = parent.world * local;
		world = idenity * local = local;
		donc update dans l'ordre suivant:
			*. parent.world (parent.local avant si necessaire, et recursivement parent.parent.etc)
				-> parent.update();
			1. local
			2. world
	*/
	Math::Vector3&	usedScale = this->_local._rescaled ? this->_local._finalScale : this->_local._scale;
	this->_local._matrix.modelMatrix(this->_local._pos, this->_local._rot, usedScale);
	//faire une method: Properties::updateMatrix() ? inline ?
	this->_world = this->_local;
	if (this->_parent) {
		this->_parent->update();
		this->_world._matrix.mult(this->_parent->_world._matrix);
	}
	/*
		voir le fonctionnement d'Unity: worldToLocalMatrix et localToWorldMatrix
	*/
}

void		Object::render(Math::Matrix4& PVmatrix) {
	if (!this->_world.isUpdated() || !this->_local.isUpdated()) {
		Math::Vector3 &	usedScale = this->_rescaled ? this->_finalScale : this->_scale;
		this->_modelMatrix.modelMatrix(this->_pos, this->_rot, usedScale);
		if (this->centered)
			this->center();
		this->_matrixUpdated = true;
	}
	//this->update();
	this->_program.render((Object&)(*this), PVmatrix);
}

//mutators
//accessors
unsigned int	Object::getId(void) const { return (this->_id); }
void			(*Object::getMotionBehaviorFunc(void) const) (Object &, void*) { return (this->_motionBehaviorFunc); }
Properties		Object::getWorldProperties() const { return (this->_world); }
Properties		Object::getLocalProperties() const { return (this->_local); }
