//#include "simplegl.h"
#include "behavior.hpp"
#include <iostream>

bool	Behavior::areActive = true;

Behavior::Behavior() {
	std::cout << "_ Behavior cons" << std::endl;
	this->isActive = true;
}

Behavior::Behavior(const Behavior& src) {
	std::cout << "_ Behavior cons by copy" << std::endl;
	*this = src;
}

Behavior& Behavior::operator=(const Behavior& src) {
	std::cout << "_ Behavior operator=" << std::endl;
	this->isActive = src.isActive;
	this->targetList = src.getTargetList();//check for bugs
	return (*this);
}

Behavior::~Behavior() {
	std::cout << "_ Behavior des" << std::endl;
	//need to delete/empty list?
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
		[target](std::pair<BehaviorManaged*, bool> elem) { std::cout << "f# set\n"; return (elem.first == target); });
	
	if (it != this->targetList.end()) {
		std::cout << "Behavior::setTargetStatus : " << it->first << std::endl;
		it->second = status;
	}
}
//accessor
bool	Behavior::getTargetStatus(BehaviorManaged* target) const {
	auto it = std::find_if(this->targetList.begin(), this->targetList.end(),
		[target](std::pair<BehaviorManaged*, bool> elem) { std::cout << "f# get\n"; return (elem.first == target); });
	
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