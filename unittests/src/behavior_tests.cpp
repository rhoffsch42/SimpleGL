#include "behavior_tests.hpp"

void	UnitTests::BehaviorTests::setUp() {
}
void	UnitTests::BehaviorTests::tearDown() {
}
void	UnitTests::BehaviorTests::testEverything() const {
	// UnitTests::streamTests << "Behavior tests" << std::endl;

	TEST("Behavior::areActive", Behavior::areActive == true);
	testBehavior();
	testBehavior_run();
	testBehavior_addTarget();
	testBehavior_removeTarget();
	testBehavior_setTargetStatus();
	testBehavior_getTargetStatus();
	testBehavior_getTargetList();
}

void	UnitTests::BehaviorTests::testBehavior() const {
	EmptyBehavior	b1;
	TEST("_ : Behavior()", b1.isActive == true);
	TEST("_ : Behavior()", b1.targetList.size() == 0);
}
void	UnitTests::BehaviorTests::testBehavior_run() const {
	EmptyBehavior	b2;
	EmptyClass		empty2;
	b2.isActive = false;

	b2.run();
	TEST("_ : Behavior run", b2.used == 0);
	b2.isActive = true;
	TEST("_ : Behavior run", b2.isActive == true);
	b2.run();
	TEST("_ : Behavior run", b2.used == 0);
	b2.addTarget(&empty2);
	b2.run();
	TEST("_ : Behavior run", b2.used == 1);

	Behavior::areActive = false;
	TEST("Behavior::areActive", Behavior::areActive == false);
	b2.run();
	TEST("_ : Behavior run", b2.used == 1);
}
void	UnitTests::BehaviorTests::testBehavior_addTarget() const {
	EmptyBehavior	b3;
	EmptyClass		empty3;
	BehaviorManaged	*managed3 = &empty3;

	b3.addTarget(NULL); //this is just a test to make isCompatible return false, not a test on NULL! normally it should crash
	TEST("_ : Behavior addTarget", b3.targetList.size() == 0);

	b3.addTarget(&empty3);
	TEST("_ : Behavior addTarget", b3.targetList.size() == 1);
	TEST("_ : Behavior addTarget", managed3->behaviorList.size() == 1);
	b3.addTarget(&empty3);
	TEST("_ : Behavior addTarget", b3.targetList.size() == 1);
	TEST("_ : Behavior addTarget", managed3->behaviorList.size() == 1);
}
void	UnitTests::BehaviorTests::testBehavior_removeTarget() const {
	EmptyBehavior	b4;
	EmptyClass		empty4;
	BehaviorManaged	*managed4 = &empty4;

	b4.addTarget(managed4);
	b4.removeTarget(managed4);
	TEST("_ : Behavior removeTarget", b4.targetList.size() == 0);
	TEST("_ : Behavior removeTarget", managed4->behaviorList.size() == 0);
	b4.removeTarget(managed4);
	TEST("_ : Behavior removeTarget", b4.targetList.size() == 0);
	TEST("_ : Behavior removeTarget", managed4->behaviorList.size() == 0);
}
void	UnitTests::BehaviorTests::testBehavior_setTargetStatus() const {
	EmptyBehavior	b5;
	EmptyClass		empty5;
	EmptyClass		empty55;

	b5.setTargetStatus(&empty5, false);
	TEST("_ : Behavior setTargetStatus", b5.targetList.size() == 0);
	b5.addTarget(&empty5);
	b5.setTargetStatus(&empty55, false);
	TEST("_ : Behavior setTargetStatus", b5.targetList.size() == 1);
	TEST("_ : Behavior setTargetStatus", b5.targetList.begin()->second == true);
	std::cout << "UnitTests setTargetStatus : " << &empty5 << std::endl;
	b5.setTargetStatus(&empty5, false);
	TEST("_ : Behavior setTargetStatus", b5.targetList.begin()->second == false);
}
void	UnitTests::BehaviorTests::testBehavior_getTargetStatus() const {
	EmptyBehavior	b6;
	EmptyClass		empty6;
	EmptyClass		empty_not_added;

	b6.addTarget(&empty6);
	TEST("_ : Behavior addTarget", b6.targetList.size() == 1);
	TEST("_ : Behavior getTargetStatus", b6.getTargetStatus(&empty6) == true);
	b6.targetList.begin()->second = false;
	TEST("_ : Behavior getTargetStatus", b6.getTargetStatus(&empty6) == false);
	TEST("_ : Behavior getTargetStatus", b6.getTargetStatus(&empty_not_added) == false);	// can't make difference between a real false, and a not found false
	NOT_TESTED("_ : Behavior getTargetStatus", " : when searching for an object not added, see behavior.cpp", UnitTests::test_error);
}
void	UnitTests::BehaviorTests::testBehavior_getTargetList() const {
	std::list< std::pair<BehaviorManaged*, bool> >	listCpy;
	EmptyBehavior	b7;
	EmptyClass		empty7;

	b7.addTarget(&empty7);
	listCpy = b7.getTargetList();
	TEST("_ : Behavior getTargetList", &listCpy != &b7.targetList);
	TEST("_ : Behavior getTargetList", listCpy.size() == b7.targetList.size());
	TEST("_ : Behavior getTargetList", listCpy.begin()->first == b7.targetList.begin()->first);
}