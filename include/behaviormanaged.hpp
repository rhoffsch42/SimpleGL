#pragma once

#include "behavior.hpp"
#include <list>

class Behavior;

class BehaviorManaged {
public:
	void	setBehaviorStatus(Behavior* be, bool status);
	void	addBehavior(Behavior* be);
	void	removeBehavior(Behavior* be);

	bool					behaviorsActive;
	std::list<Behavior*>	behaviorList;//private?

protected://to avoid this class to be instanciated by its own, put all constructors here
	BehaviorManaged();
	BehaviorManaged(const BehaviorManaged& src);
	BehaviorManaged&	operator=(const BehaviorManaged& src);//todo
	virtual ~BehaviorManaged();
};