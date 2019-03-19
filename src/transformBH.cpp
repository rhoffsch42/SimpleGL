#include "simplegl.h"
#include "transformBH.hpp"

bool	TransformBH::areActive = true;

TransformBH::TransformBH() : Behavior() {
	cout << "_ TransformBH cons" << endl;
	this->modePos = ADDITIVE;
	this->modeRot = ADDITIVE;
	this->modeScale = ADDITIVE;
	cout << "Cons this->targetList addr: " << &this->targetList << endl;
}


TransformBH::TransformBH(t_pp transformValues) : Behavior(), transform(transformValues) {
	cout << "_ TransformBH cons" << endl;
	this->modePos = ADDITIVE;
	this->modeRot = ADDITIVE;
	this->modeScale = ADDITIVE;
}

TransformBH::TransformBH(Math::Vector3 pos, Math::Rotation rot, Math::Vector3 scale) : Behavior() {
	cout << "_ TransformBH cons by copy" << endl;
	this->transform.pos = pos;
	this->transform.rot = rot;
	this->transform.scale = scale;
	this->modePos = ADDITIVE;
	this->modeRot = ADDITIVE;
	this->modeScale = ADDITIVE;
}

TransformBH::TransformBH(const TransformBH& src) {
	cout << "_ TransformBH cons by copy" << endl;
	*this = src;
}

TransformBH&	TransformBH::operator=(const TransformBH& src) {
	Behavior::operator=(src);
	cout << "_ TransformBH operator=" << endl;
	this->transform = src.transform;
	this->modePos = src.modePos;
	this->modeRot = src.modeRot;
	this->modeScale = src.modeScale;
	return (*this);
}

TransformBH::~TransformBH() {
	cout << "_ TransformBH des" << endl;
	//need to delete/empty list? parent des is called ?
}

void	TransformBH::run() {
	// cout << "_ TransformBH::run" << endl;
	//	check TransformBH::areActive and Behavior::AreActive here ?
	for (auto i : this->targetList) {
		Object*	target = dynamic_cast<Object*>(i.first);//specialisation part
		if (i.second) {//check if behavior status for this target
			if (this->modePos == ADDITIVE) {
				target->local.translate(this->transform.pos);
			} else {
				Math::Vector3	newpos = target->local.getPos();
				newpos.x *= this->transform.pos.x;
				newpos.y *= this->transform.pos.y;
				newpos.z *= this->transform.pos.z;
				target->local.setPos(newpos);
			}

			if (this->modeRot == ADDITIVE) {
				target->local.rotate(this->transform.rot);
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
				Math::Rotation	newrot = target->local.getRot();
				newrot.setAsRad();
				this->transform.rot.setAsRad();
				newrot.x *= this->transform.rot.x;
				newrot.y *= this->transform.rot.y;
				newrot.z *= this->transform.rot.z;
				target->local.setRot(newrot);
			}

			if (this->modeScale == ADDITIVE) {
				target->local.enlarge(this->transform.scale);
			} else {
				Math::Vector3	newscale = target->local.getScale();
				newscale.x *= this->transform.scale.x;
				newscale.y *= this->transform.scale.y;
				newscale.z *= this->transform.scale.z;
				target->local.setScale(newscale);
			}
		}
	}
}

bool	TransformBH::isCompatible(BehaviorManager* target) const {
	//dynamic_cast check
	(void)target;
	return (true);
}