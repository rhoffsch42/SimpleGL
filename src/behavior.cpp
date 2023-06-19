#include "simplegl.h"
#include "behavior.hpp"
#include "compiler_settings.h"
#include <iostream>

#ifdef SGL_DEBUG
 #define SGL_BEHAVIOR_DEBUG
#endif
#ifdef SGL_BEHAVIOR_DEBUG 
 #define D(x) std::cout << "[Behavior] " << x ;
 #define D_(x) x
 #define D_SPACER "-- behavior.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

bool	Behavior::areActive = true;

Behavior::Behavior() {
	D("Behavior cons" << std::endl)
	this->isActive = true;
}

Behavior::Behavior(const Behavior& src) {
	D("Behavior cons by copy" << std::endl)
	*this = src;
}

Behavior& Behavior::operator=(const Behavior& src) {
	D("Behavior operator=" << std::endl)
	this->isActive = src.isActive;
	this->targetList = src.getTargetList();//check for bugs
	return (*this);
}

Behavior::~Behavior() {
	D("Behavior des" << std::endl)
	//need to delete/empty list?
	//should remove itseft from its target if they are managed
	for (auto i : this->targetList) {
		this->removeTarget(i.first);
	}
}

void	Behavior::run() {
	if (Behavior::areActive && this->isActive) {
		for (auto i : this->targetList) {
			if (i.second && i.first->behaviorsActive) {
				this->behaveOnTarget(i.first);
			}
		}
	}
}

	//make this a template! (another)
void	Behavior::addTarget(BehaviorManaged* target) {
	if (this->isCompatible(target)) {
		if (std::find_if(this->targetList.begin(), this->targetList.end(), 
				[target](std::pair<BehaviorManaged*, bool> elem) { return (elem.first == target); })
			== this->targetList.end()) {
			std::pair<BehaviorManaged*, bool>	p = std::pair<BehaviorManaged*, bool>(target, true);//cast
			this->targetList.push_back(p);
			/*
				we can check here if the target has a BehaviorManaged, and add ourself from his list:
				if (hasTheManager) {
					target->addBehavior(this);
				}
				edit: it should always have it!
			*/
			target->addBehavior(this);
		}
	}

}

	//make this a template! (another)
void	Behavior::removeTarget(BehaviorManaged* target) {
	this->targetList.erase(std::remove_if(this->targetList.begin(), this->targetList.end(),
		[&target](std::pair<BehaviorManaged*, bool> elem) { return (elem.first == target); }),
		this->targetList.end());
	/*
		we can check here if the target has a BehaviorManaged, and remove ourself from his list:
		if (hasTheManager) {
			target->removeBehavior(this);
		}
		edit: it should always have it!
	*/
	target->removeBehavior(this);
}

void	Behavior::setTargetStatus(BehaviorManaged* target, bool status) {
	auto it = std::find_if(this->targetList.begin(), this->targetList.end(),
		[target](std::pair<BehaviorManaged*, bool> elem) { D("f# set\n") return (elem.first == target); });
	
	if (it != this->targetList.end()) {
		D("Behavior::setTargetStatus : " << it->first << std::endl)
		it->second = status;
	}
}
//accessor
bool	Behavior::getTargetStatus(BehaviorManaged* target) const {
	auto it = std::find_if(this->targetList.begin(), this->targetList.end(),
		[target](std::pair<BehaviorManaged*, bool> elem) { D("f# get\n") return (elem.first == target); });
	
	if (it != this->targetList.end()) {
		return (it->second);
	}
	/*
		What if not found ?
		- should throw an error?
	 	- return a copied pair? ptr to NULL if not found
	*/
	return (false);
}

std::list< std::pair<BehaviorManaged*, bool> >	Behavior::getTargetList() const { return (this->targetList); }