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
	this->_local = src.getLocalProperties();
	this->_worldMatrix = Math::Matrix4(src.getWorldMatrix());

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

bool		Object::update() {//update Properties
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
	bool	localUpdated = this->_local.updateMatrix();
	if (this->_parent) {
		bool	parentUpdated = this->_parent->update();
		if (!localUpdated || !parentUpdated) {
			this->_worldMatrix = this->_parent->_worldMatrix;
			this->_worldMatrix.mult(this->_local._matrix);// mult inverse ?
			return (false);
		}
	}
	else if (!localUpdated) {
		this->_worldMatrix = this->_local._matrix;
		return (false);
	}
	else
		return (true);
	/*
		voir le fonctionnement d'Unity: worldToLocalMatrix et localToWorldMatrix ?
	*/
}

void		Object::render(Math::Matrix4& PVmatrix) {
	this->update();
	// this->_program.render((Object&)(*this), PVmatrix);
}

//mutators
//accessors
unsigned int	Object::getId(void) const { return (this->_id); }
void			(*Object::getMotionBehaviorFunc(void) const) (Object &, void*) { return (this->_motionBehaviorFunc); }
Properties		Object::getLocalProperties() const { return (this->_local); }
Math::Matrix4&	Object::getWorldMatrix() const { return ((Math::Matrix4&)this->_worldMatrix); }
