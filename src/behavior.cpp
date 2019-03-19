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

	//make this a template! (another)
void	Behavior::addTarget(BehaviorManaged* target) {
	if (this->isCompatible(target)) {
		if (std::find_if(this->targetList.begin(), this->targetList.end(), 
				[target](std::pair<BehaviorManaged*, bool> elem) { return (elem.first == target); })
			== this->targetList.end()) {
			std::pair<BehaviorManaged*, bool>	p = pair<BehaviorManaged*, bool>(target, true);//cast
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
		[&target](std::pair<BehaviorManaged*, bool> elem) { return (&elem.first == &target); });
	(*it).second = status;
}
//accessor
std::list< std::pair<BehaviorManaged*, bool> >	Behavior::getTargetList() const { return (this->targetList); }