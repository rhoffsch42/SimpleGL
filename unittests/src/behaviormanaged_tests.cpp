#include "behaviormanaged_tests.hpp"

void	UnitTests::BehaviorManagedTests::setUp() {
	emptyA_1bh_false.addBehavior(&bh_printAdress);
	emptyA_1bh_false.behaviorsActive = false;
}
void	UnitTests::BehaviorManagedTests::tearDown() {
}
void	UnitTests::BehaviorManagedTests::testEverything() const {
	// UnitTests::streamTests << "BehaviorManaged tests" << std::endl;
	testBehaviorManaged();
	testBehaviorManaged_operatorEqual();
	testBehaviorManaged_setBehaviorStatus();
	testBehaviorManaged_addBehavior();
	testBehaviorManaged_removeBehavior();
}

void	UnitTests::BehaviorManagedTests::testBehaviorManaged() const {
	EmptyClass	empty1;
	TEST("_ : BehaviorManaged", empty1.behaviorsActive == true);
	TEST("_ : BehaviorManaged", empty1.behaviorList.size() == 0);
}
void	UnitTests::BehaviorManagedTests::testBehaviorManaged_operatorEqual() const {
	EmptyClass	empty2 = emptyA_1bh_false;
	
	TEST("_ : BehaviorManaged", empty2.behaviorsActive == emptyA_1bh_false.behaviorsActive);
	TEST("_ : BehaviorManaged", empty2.behaviorList.size() == emptyA_1bh_false.behaviorList.size());
	NOT_TESTED("_ : BehaviorManaged", " : need to check that the list is well copied (structurally equal, but instances are different)", UnitTests::test_human);
}
void	UnitTests::BehaviorManagedTests::testBehaviorManaged_setBehaviorStatus() const {
	EmptyClass	empty3;
	EmptyBehavior	bh_printAdresstmp;
	Behavior*	be = &bh_printAdresstmp;
	empty3.addBehavior(be);
	empty3.setBehaviorStatus(be, false);
	bool	object_found = false;
	for (auto i : bh_printAdress.targetList) {
		if (i.first == &empty3) {
			TEST("_ : BehaviorManaged setBehaviorStatus", i.second == false);
			object_found = true;
		}
	}
	TEST("_ : BehaviorManaged setBehaviorStatus", object_found == true);
	// TEST("_ : BehaviorManaged setBehaviorStatus", empty2.behaviorsActive == emptyA_1bh_false.behaviorsActive);
}
void	UnitTests::BehaviorManagedTests::testBehaviorManaged_addBehavior() const {
	EmptyClass	empty4;
	EmptyBehavior	bh_printAdresstmp;
	Behavior*	be = &bh_printAdresstmp;

	empty4.addBehavior(be);
	TEST("_ : BehaviorManaged addBehavior", empty4.behaviorList.size() == 1);
	TEST("_ : BehaviorManaged addBehavior", be->targetList.size() == 1);
	empty4.addBehavior(be);
	TEST("_ : BehaviorManaged (re)addBehavior", empty4.behaviorList.size() == 1);
	TEST("_ : BehaviorManaged (re)addBehavior", be->targetList.size() == 1);
	NOT_TESTED("_ : BehaviorManaged addBehavior(NULL)", " : todo", UnitTests::test_error);
}
void	UnitTests::BehaviorManagedTests::testBehaviorManaged_removeBehavior() const {
	EmptyClass	empty5;
	EmptyBehavior	bh_printAdresstmp;
	Behavior*	be = &bh_printAdresstmp;

	empty5.addBehavior(be);
	empty5.removeBehavior(be);
	TEST("_ : BehaviorManaged removeBehavior", empty5.behaviorList.size() == 0);
	TEST("_ : BehaviorManaged removeBehavior", be->targetList.size() == 0);
	empty5.removeBehavior(be);
	TEST("_ : BehaviorManaged (re)removeBehavior", empty5.behaviorList.size() == 0);
	TEST("_ : BehaviorManaged (re)removeBehavior", be->targetList.size() == 0);
}
