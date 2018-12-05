#pragma once

#include <list>
#include <utility>
#include <algorithm>

#include "object.hpp"
//include fps.h

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

	virtual void	run() = 0;
	virtual void	addTarget(const Object& target) = 0;//can be something else than Object?
	void			removeTarget(const Object& target);
	void			setTargetStatus(const Object& target, bool status);
	std::list< std::pair<Object&, bool> >	getTargetList() const;


	bool	isActive;
	std::list< std::pair<Object&, bool> >	targetList;
};

/*
	Objectifs:
		-	appliquer un behavior a un object
		-	modifier les param d'un behavior pour un seul object
		-	modifier les param d'un behavior pour tous les objects
		-	desactiver le behavior pour un seul object
		-	desactiver le behavior pour tous les objects
		+	force un behavior pour tous ses objects linkes, ou le(s) behavior(s) d'un seul object
			via liste(s) d'exceptions

		ex:	
			//	in class Behavior:
		{
			list< pair<bool, Object> >	targetList;
		}
			///////////////
		{
			Object		o1, o2, o3, o4, o5;
			GrowAndShrink	b1;
			b1.growTime = 5.0f;
			GrowAndShrink	b2; 
			b2.growTime = 10.0f;
			GrowAndShrink	b3 = b2;
			b3.repeat = false;

			b1.addTarget(o1, o2, o3);
			b2.addTarget(o4, o5);

		//	check inheritence with static variables: private/protected/public
			Behavior::areActive = false;		//	deactivate all Behaviors
			GrowAndShrink::areActive = true;	//	activate all GrowAndShrink (works if Behavior::areActive = true;)

			b1.isActive = false;				//	deactivate only b1;
			
			o5.BehaviorsActivated = false;		//	don't run all Behaviors linked
		
		<=>	b2.deactivateObject(o4);			//	see comment below
		<=>	o4.deactivateBehavior(b2);			//	"
			o4.behaviorsActive = false
		</--	
			to pause the behavior for a certain target, but not pause it for other objects:
			find the object in the behavior targetList, and set to false the bool in the pair<bool, Object>
		--/>
			
			b2.removeTarget(o5);				//	remove the object from the behavior;
			delete b1;
		}


		*	comment savoir si l'object a un behavior si on ne stocke pas ce dernier ?
			-> std::list<Object>::iterator	it = std::find(list.begin(), list.end(), target);
			/!\ fastidieux, on doit donc le faire sur toutes les list des behaviors
				-> stocker le behavior dans l'object:	list<Behavior>	*beList;
				target.beList
*/