#include "simplegl.h"
#include "behavior.hpp"

bool	Behavior::areActive = true;

Behavior::Behavior() {
	cout << "_ Behavior cons" << endl;
	this->isActive = true;
}

Behavior::Behavior(const Behavior& src) {
	cout << "_ Behavior cons by copy" << endl;
	*this = src;
}

Behavior&	Behavior::operator=(const Behavior& src) {
	cout << "_ Behavior operator=" << endl;
	this->isActive = src.isActive;
	this->targetList = src.getTargetList();//check for bugs
	return (*this);
}

Behavior::~Behavior() {
	cout << "_ Behavior des" << endl;
	//need to delete/empty list?
}

	//make this a template!
void	Behavior::addTarget(BehaviorManager* target) {
	if (this->isCompatible(target)) {
		if (std::find_if(this->targetList.begin(), this->targetList.end(), 
				[target](std::pair<BehaviorManager*, bool> elem) { return (elem.first == target); })
			== this->targetList.end()) {
			std::pair<BehaviorManager*, bool>	p = pair<BehaviorManager*, bool>(target, true);//cast
			this->targetList.push_back(p);
		}
	}
	/*
		we can check here if the target has a BehaviorManager, and add ourself from his list:
		if (hasTheManager) {
			target->addBehavior(this);
		}
	*/
}

	//make this a template!
void	Behavior::removeTarget(BehaviorManager* target) {
	this->targetList.erase(std::remove_if(this->targetList.begin(), this->targetList.end(),
		[&target](std::pair<BehaviorManager*, bool> elem) { return (elem.first == target); }),
		this->targetList.end());
	/*
		we can check here if the target has a BehaviorManager, and remove ourself from his list:
		if (hasTheManager) {
			target->removeBehavior(this);
		}
	*/
}

void	Behavior::setTargetStatus(BehaviorManager* target, bool status) {
	auto it = std::find_if(this->targetList.begin(), this->targetList.end(),
		[&target](std::pair<BehaviorManager*, bool> elem) { return (&elem.first == &target); });
	(*it).second = status;
}
//accessor
std::list< std::pair<BehaviorManager*, bool> >	Behavior::getTargetList() const { return (this->targetList); }