#pragma once

#include "behaviormanaged.hpp"
#include <list>
#include <utility>
#include <algorithm>

/*
	Objectifs:
		+	appliquer un behavior a un object
		+	modifier les param d'un behavior pour un seul object  (copy et modify le BH, puis add l'objet)
		+	modifier les param d'un behavior pour tous les objects
		+	desactiver le behavior pour un seul object
		+	desactiver le behavior pour tous les objects
		-	force un behavior pour tous ses objects linkes, ou le(s) behavior(s) d'un seul object
			via liste(s) d'exceptions
*/
class BehaviorManaged;

class Behavior {
public:
	static bool						areActive;
	// static std::list<Behavior&>		currentBehaviors;
	// static void						addBehavior(const Behavior& b);
	// static void						removeBehavior(const Behavior& b);

	Behavior();
	Behavior(const Behavior& src);
	virtual Behavior&	operator=(const Behavior& src);
	~Behavior();

	virtual void	run() = 0; // should be const ? can be restrictive, see transformBH.cpp
	virtual	bool	isCompatible(BehaviorManaged* target) const = 0; // decides if we can add target
			void	addTarget(BehaviorManaged* target);
			void	removeTarget(BehaviorManaged* target);
			void	setTargetStatus(BehaviorManaged* target, bool status);
	std::list< std::pair<BehaviorManaged*, bool> >	getTargetList() const;//return a copy!

	bool	isActive;
	std::list< std::pair<BehaviorManaged*, bool> >	targetList;
};
/*
	#include "behavior.cpp" // this is needed if we use template
	https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
*/
/*
	lambda expression:
	https://stackoverflow.com/questions/17965728/find-a-pair-in-a-stl-list-where-only-first-element-is-known
	remove_if:
	https://stackoverflow.com/questions/22729906/stdremove-if-not-working-properly/22729987#22729987
	it just move the eligible one to the end, and return a pointer to the start of the moved stuff
*/
/*
	why target MUST be BehaviorManaged* , and not void*
	https://stackoverflow.com/questions/4131091/dynamic-cast-from-void
	and it CANNOT be Object*, because Object is an incomplete type atm
		Object : BehaviorManaged, BehaviorManaged would need Object that is derived from BehaviorManaged...
*/
