#include "simplegl.h"
#include "compiler_settings.h"
#include "object.hpp"

#ifdef SGL_DEBUG
 #define SGL_OBJECT_DEBUG
#endif
#ifdef SGL_OBJECT_DEBUG 
 #define D(x) std::cout << "[Object] " << x ;
 #define D_(x) x
 #define D_SPACER "-- object.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

//static variables initialisation
unsigned int	Object::getInstanceAmount() { return (Object::_instanceAmount); }
unsigned int	Object::_instanceAmount = 0;
unsigned int	Object::_instanceId = 0;

Object::Object() : BehaviorManaged() {
	//D("Object cons" << endl)
	this->_id = Object::_instanceId;
	this->_parent = nullptr;
	this->_worldMatrixChanged = true;

	Object::_instanceAmount++;
	Object::_instanceId++;
}

Object::Object(Properties object_pp) : BehaviorManaged(), local(object_pp) {
	D("Object cons with custom Properties" << std::endl)
	this->_id = Object::_instanceId;
	this->_parent = nullptr;
	this->_worldMatrixChanged = true;

	Object::_instanceAmount++;
	Object::_instanceId++;
}

Object::Object(const Object& src) {
	D("Object cons by copy" << std::endl)

	*this = src;
}

Object&		Object::operator=(const Object& src) {
	this->_id = Object::_instanceId;
	this->local = src.local;
	this->_worldMatrix = Math::Matrix4(src._worldMatrix);
	this->_worldMatrixChanged = true;
	this->_parent = src._parent;

	Object::_instanceAmount++;
	Object::_instanceId++;
	return (*this);
}

Object::~Object() {
	//D("Object des" << endl)
	Object::_instanceAmount--;
	//remove iteself from behaviors! todo
}

// true = was already updated ; false = wasn't update
bool		Object::update() {//update Properties
	//D("* Object::update" << endl)
	this->local.updateMatrix();
	if (this->_parent) {
		this->_parent->update();
		// if (this->_parent->local._matrixChanged || this->local._matrixChanged) {
		if (this->_parent->_worldMatrixChanged || this->_parent->local._matrixChanged || this->local._matrixChanged) {
			if (1) {
				this->_worldMatrix = this->_parent->_worldMatrix;
				this->_worldMatrix.mult(this->local._matrix);
			} else {
				this->_worldMatrix = this->local._matrix;
				this->_worldMatrix.mult(this->_parent->_worldMatrix);
			}
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

void		Object::render(Math::Matrix4& PVmatrix) {
	this->update();
	(void)PVmatrix;
	//D("Empty Object: nothing to render." << endl)
}

//mutators
void			Object::setParent(Object* parent) {
	this->_parent = parent;
}
//accessors
unsigned int	Object::getId(void) const { return (this->_id); }
Properties		Object::getLocalProperties() const { return (this->local); }
Math::Matrix4&	Object::getWorldMatrix() const { return ((Math::Matrix4&)this->_worldMatrix); }
Object*			Object::getParent() const { return (this->_parent); }
