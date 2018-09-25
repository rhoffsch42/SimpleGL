#include "simplegl.h"
#include "object.hpp"

//static variables initialisation
unsigned int	Object::getInstanceAmount() { return (Object::_instanceAmount); }
unsigned int	Object::_instanceAmount = 0;
unsigned int	Object::_instanceId = 0;

Object::Object() {
	cout << "_ Object cons" << endl;
	this->_id = Object::_instanceId;
	this->_motionBehavior = false;
	this->_motionBehaviorFunc = NULL;
	this->_parent = NULL;
	this->_worldMatrixChanged = true;

	Object::_instanceAmount++;
	Object::_instanceId++;
}

Object::Object(Properties object_pp) : local(object_pp) {
	cout << "_ Object cons with custom Properties" << endl;
	this->_id = Object::_instanceId;
	this->_motionBehavior = false;
	this->_motionBehaviorFunc = NULL;
	this->_parent = NULL;
	this->_worldMatrixChanged = true;

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
	this->local = src.getLocalProperties();
	this->_worldMatrix = Math::Matrix4(src.getWorldMatrix());
	this->_worldMatrixChanged = true;

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
	// this->local.updateMatrix();
	// if (this->_parent) {
	// 	this->_parent->update();
	// 	if (0) {
	// 		this->_worldMatrix = this->local._matrix;
	// 		this->_worldMatrix.mult(this->_parent->_worldMatrix);
	// 	}
	// 	else {
	// 		this->_worldMatrix = this->_parent->_worldMatrix;
	// 		this->_worldMatrix.mult(this->local._matrix);// mult inverse ?
	// 	}
	// }
	// else
	// {
	// 	this->_worldMatrix = this->local._matrix;
	// }
	// return (false);


	this->local.updateMatrix();
	if (this->_parent) {
		this->_parent->update();
		// if (this->_parent->local._matrixChanged || this->local._matrixChanged) {
		if (this->_parent->_worldMatrixChanged || this->_parent->local._matrixChanged || this->local._matrixChanged) {
			this->_worldMatrix = this->_parent->_worldMatrix;
			this->_worldMatrix.mult(this->local._matrix);
			this->_worldMatrixChanged = true;
			return (false);
		}
	}
	else if (this->local._matrixChanged) {
		this->_worldMatrix = this->local._matrix;
		this->_worldMatrixChanged = true;
		return (false);
	}
	return (true);
	/*
		voir le fonctionnement d'Unity: worldToLocalMatrix et localToWorldMatrix ?
	*/
}

// void		Object::render(Math::Matrix4& PVmatrix) {
// 	this->update();
// 	// this->_program.render((Object&)(*this), PVmatrix);
// }

//mutators
void			Object::setParent(Object* parent) {
	this->_parent = parent;
}
//accessors
unsigned int	Object::getId(void) const { return (this->_id); }
void			(*Object::getMotionBehaviorFunc(void) const) (Object &, void*) { return (this->_motionBehaviorFunc); }
Properties		Object::getLocalProperties() const { return (this->local); }
Math::Matrix4&	Object::getWorldMatrix() const { return ((Math::Matrix4&)this->_worldMatrix); }
Object*			Object::getParent() const { return (this->_parent); }
