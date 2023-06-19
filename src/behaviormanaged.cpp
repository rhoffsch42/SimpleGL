#include "behaviormanaged.hpp"
#include "simplegl.h"
#include "compiler_settings.h"
#include <iostream>

#ifdef SGL_DEBUG
 #define SGL_BEHAVIOR_MANAGED_DEBUG
#endif
#ifdef SGL_BEHAVIOR_MANAGED_DEBUG 
 #define D(x) std::cout << "[BehaviorManaged] " << x ;
 #define D_(x) x
 #define D_SPACER "-- behaviormanaged.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

BehaviorManaged::BehaviorManaged() {
	//D("BehaviorManaged cons" << std::endl)
	this->behaviorsActive = true;
}

BehaviorManaged::BehaviorManaged(const BehaviorManaged& src) {
	this->behaviorsActive = src.behaviorsActive;
	this->behaviorList = src.behaviorList;//copy ?
	D("test BehaviorManaged operator=" << std::endl)
	D(&this->behaviorList << std::endl)
	D(&this->behaviorList << std::endl)
	// exit(0);
}

BehaviorManaged::~BehaviorManaged() {
	//	removes itself from all its Behaviors's list
	for (auto i : this->behaviorList) {
		D("removing bh : " << i << std::endl)
		i->removeTarget(this);
		D("removing done : " << i << std::endl)
	}
	//	erase/empty list ?
}

void	BehaviorManaged::setBehaviorStatus(Behavior* be, bool status) {
	be->setTargetStatus(this, status);
}

//make this a template! (another)
void	BehaviorManaged::addBehavior(Behavior* be) {
	if (std::find_if(this->behaviorList.begin(), this->behaviorList.end(), 
	[be](Behavior* elem) { return (elem == be); }) 
	== this->behaviorList.end()) {
		this->behaviorList.push_back(be);
		be->addTarget(this);
	}
}

//make this a template! (another)
void	BehaviorManaged::removeBehavior(Behavior* be) {
	auto it = std::remove_if(this->behaviorList.begin(), this->behaviorList.end(),
		[be](Behavior* elem) { return (elem == be); });
	if (it != this->behaviorList.end()) {
		this->behaviorList.erase(it, this->behaviorList.end());
		be->removeTarget(this);
	}
}

bool					behaviorsActive;
std::list<Behavior*>	behaviorList;//private?

