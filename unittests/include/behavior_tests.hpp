#pragma once

/*
	It is important to declare the prerequired classes before including unittests.hpp
*/
#include "behavior.hpp"
#include "behaviormanaged.hpp"
class EmptyClass : public BehaviorManaged {
public:
	int	value;	
};

class EmptyBehavior : public Behavior {
public:
	EmptyBehavior() : Behavior() {
		this->used = 0;
	}
	void	behaveOnTarget(BehaviorManaged* target) {
		std::cout << "doing nothing on the classes, here is the adress: " << target << std::endl;
		this->used++;
	}
	bool	isCompatible(BehaviorManaged* target) const {
		if (!target) {
			std::cout << "OMG shitty NULL \n";
			return (false);
		}
		return (true);
	}
	unsigned int	used;
};

#include "unittests.hpp"

class UnitTests::BehaviorTests {
public:
	void	setUp();
	void	tearDown();
	void	testEverything() const;

	void	testBehavior() const;
	void	testBehavior_run() const;
	void	testBehavior_addTarget() const;
	void	testBehavior_removeTarget() const;
	void	testBehavior_setTargetStatus() const;
	void	testBehavior_getTargetStatus() const;// do that !
	void	testBehavior_getTargetList() const;
	// void	testBehavior_() const;
private:
	EmptyBehavior	bh_printAdress;
	EmptyClass		emptyA;
};
