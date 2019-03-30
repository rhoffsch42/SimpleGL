#pragma once

#include "unittests.hpp"
#include "behaviormanaged.hpp"

class EmptyClass : public BehaviorManaged {
};

class EmptyBehavior : public Behavior {
	void	run() {
		std::cout << "doing nothing on the classes, here are there adresses:" << std::endl;
		for (auto i : this->targetList) {
			std::cout << &i.first << std::endl;
		}
	}
	bool	isCompatible(BehaviorManaged* target) const {
		if (!target)
			std::cout << "OMG shitty NULL \n";
		return (true);
	}
};

class UnitTests::BehaviorManagedTests {
public:
	void	setUp();
	void	tearDown();
	void	testEverything() const;

	void	testBehaviorManaged() const;
	void	testBehaviorManaged_operatorEqual() const;
	void	testBehaviorManaged_setBehaviorStatus() const;
	void	testBehaviorManaged_addBehavior() const;
	void	testBehaviorManaged_removeBehavior() const;

private:
	EmptyBehavior	bh_printAdress;
	EmptyClass		emptyA_1bh_false;
};
