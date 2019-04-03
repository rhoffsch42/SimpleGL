#pragma once

#include "unittests.hpp"
#include "behaviormanaged.hpp"

/*
	include behavior_tests.hpp for
	EmptyBehavior : Behavior
	EmptyClass : BehaviorManaged
*/
#include "behavior_tests.hpp"

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
