#include "behaviormanaged.hpp"
#include <iostream>

BehaviorManaged::BehaviorManaged() {
	//std::cout << "_ BehaviorManaged cons" << std::endl;
	this->behaviorsActive = true;
}

BehaviorManaged::BehaviorManaged(const BehaviorManaged& src) {
	this->behaviorsActive = src.behaviorsActive;
	this->behaviorList = src.behaviorList;//copy ?
	std::cout << "test BehaviorManaged operator=" << std::endl;
	std::cout << &this->behaviorList << std::endl;
	std::cout << &this->behaviorList << std::endl;
	// exit(0);
}

BehaviorManaged::~BehaviorManaged() {
	//	removes itself from all its Behaviors's list
	for (auto i : this->behaviorList) {
		i->removeTarget(this);
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

