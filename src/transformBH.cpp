#include "simplegl.h"
#include "transformBH.hpp"

#ifdef SGL_DEBUG
 #define SGL_TRANSFORM_BH_DEBUG
#endif
#ifdef SGL_TRANSFORM_BH_DEBUG 
 #define D(x) std::cout << "[TransformBH] " << x ;
 #define D_(x) x
 #define D_SPACER "-- transformBH.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif


bool	TransformBH::areActive = true;

TransformBH::TransformBH() : Behavior() {
	D("TransformBH cons" << std::endl)
	this->modePos = ADDITIVE;
	this->modeRot = ADDITIVE;
	this->modeScale = ADDITIVE;
	D("Cons this->targetList addr: " << &this->targetList << std::endl)
}


TransformBH::TransformBH(t_pp transformValues) : Behavior(), transform(transformValues) {
	D("TransformBH cons" << std::endl)
	this->modePos = ADDITIVE;
	this->modeRot = ADDITIVE;
	this->modeScale = ADDITIVE;
}

TransformBH::TransformBH(Math::Vector3 pos, Math::Rotation rot, Math::Vector3 scale) : Behavior() {
	D("TransformBH cons by copy" << std::endl)
	this->transform.pos = pos;
	this->transform.rot = rot;
	this->transform.scale = scale;
	this->modePos = ADDITIVE;
	this->modeRot = ADDITIVE;
	this->modeScale = ADDITIVE;
}

TransformBH::TransformBH(const TransformBH& src) {
	D("TransformBH cons by copy" << std::endl)
	*this = src;
}

TransformBH& TransformBH::operator=(const TransformBH& src) {
	Behavior::operator=(src);
	D("TransformBH operator=" << std::endl)
	this->transform = src.transform;
	this->modePos = src.modePos;
	this->modeRot = src.modeRot;
	this->modeScale = src.modeScale;
	return (*this);
}

TransformBH::~TransformBH() {
	D("TransformBH des" << std::endl)
	//need to delete/empty list? parent des is called ?
}

void	TransformBH::behaveOnTarget(BehaviorManaged* target) {
	//D("TransformBH::run" << std::endl)
	Object*	speTarget = dynamic_cast<Object*>(target);//specialisation part //todo check if success (should always be? as it was checked on isCompatible())
	if (this->modePos == ADDITIVE) {
		speTarget->local.translate(this->transform.pos);
	} else {
		Math::Vector3	newpos = speTarget->local.getPos();
		newpos.x *= this->transform.pos.x;
		newpos.y *= this->transform.pos.y;
		newpos.z *= this->transform.pos.z;
		speTarget->local.setPos(newpos);
	}

	if (this->modeRot == ADDITIVE) {
		speTarget->local.rotate(this->transform.rot);
	} else {
	/*
		can have weird results? as rotations can have equivalent values:
		in degrees:
		-10 = +350
		-10x2 = +350x2
		-20 = +700 (%360)
		-20 = +340
		apparently working ...
	*/
		Math::Rotation	newrot = speTarget->local.getRot();
		newrot.setAsRad();// speTarget and behavior have to have the same unit for rot
		this->transform.rot.setAsRad();// "
		newrot.x *= this->transform.rot.x;
		newrot.y *= this->transform.rot.y;
		newrot.z *= this->transform.rot.z;
		speTarget->local.setRot(newrot);
	}

	if (this->modeScale == ADDITIVE) {
		speTarget->local.enlarge(this->transform.scale);
	} else {
		Math::Vector3	newscale = speTarget->local.getScale();
		newscale.x *= this->transform.scale.x;
		newscale.y *= this->transform.scale.y;
		newscale.z *= this->transform.scale.z;
		speTarget->local.setScale(newscale);
	}
}

bool	TransformBH::isCompatible(BehaviorManaged* target) const {
	//dynamic_cast check
	(void)target;
	return (true);
}
