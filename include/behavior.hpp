#pragma once

#include <list>
#include <utility>
#include <algorithm>

//include fps.h

/*
	lambda expression:
	https://stackoverflow.com/questions/17965728/find-a-pair-in-a-stl-list-where-only-first-element-is-known
	remove_if:
	https://stackoverflow.com/questions/22729906/stdremove-if-not-working-properly/22729987#22729987
	it just move the eligible one to the end, and return a pointer to the start of the moved stuff
*/
/*
	why target MUST be BehaviorManager* (managed), and not void*
	https://stackoverflow.com/questions/4131091/dynamic-cast-from-void
	and it CANNOT be Object*, because Object is an incomplete type atm
		Object : BehaviorManager, BehaviorManager would need Object that is derived from BehaviorManager...

	-> we could use template list:
		template<T>
		class Behavior {
			...
			std::list< std::pair<T*, bool> >	targetList;
		}
	to then do:
		TransformBH<Object*>	b1;
	it implies that all targets must have a common Base class (above example is Object)
	so why not make all targets derived from BehaviorManager directly? ...
*/

class BehaviorManager;

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
			void	addTarget(BehaviorManager* target);
	virtual	bool	isCompatible(BehaviorManager* target) const = 0; // decides if we can add target
			void	removeTarget(BehaviorManager* target);
			void	setTargetStatus(BehaviorManager* target, bool status);
	std::list< std::pair<BehaviorManager*, bool> >	getTargetList() const;//return a copy!

	bool	isActive;
	std::list< std::pair<BehaviorManager*, bool> >	targetList;
};
//#include "behavior.cpp" // this is needed for template!
// https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file

/*
	transofrm Behavior with pointer (void) then specialise TransformBH in Object* (cast)
	then we can create the BehaviorManager that will allow Object to manage their behavior
		* Object and TransformBH are just examples
		/!\	dangerous cauz if we do:	T->addTarget( (void*)(thing) )
			thing must be the good type for T, if not: undefined behavior
			compiler can't know if it's ok to do that
*/

class BehaviorManager {
public:
	void	setBehaviorStatus(Behavior* be, bool status) {
		be->setTargetStatus(this, status);
	}
	//make this a template! (another)
	void	addBehavior(Behavior* be) {
		if (std::find_if(this->behaviorList.begin(), this->behaviorList.end(), 
		[be](Behavior* elem) { return (elem == be); }) 
		== this->behaviorList.end()) {
			this->behaviorList.push_back(be);
			be->addTarget(this);
			/*
				i dont know if it's ok, as this class should always be inherited
				ex: class Object : BehaviorManager { [...] }
				hope 'this' is type of Object* 
			*/
		}
	}
	//make this a template! (another)
	void	removeBehavior(Behavior* be) {
		auto it = std::remove_if(this->behaviorList.begin(), this->behaviorList.end(),
			[be](Behavior* elem) { return (elem == be); });
		if (it != this->behaviorList.end()) {
			this->behaviorList.erase(it, this->behaviorList.end());
			be->removeTarget(this);
		}
		/*
			same problem as above
		*/
	}

	bool					behaviorsActive;
	std::list<Behavior*>	behaviorList;

protected://to avoid this class to be instanciated by its own, put all constructors here
	BehaviorManager() {	}
	BehaviorManager(const BehaviorManager& src) {
		this->behaviorsActive = src.behaviorsActive;
		this->behaviorList = src.behaviorList;//copy ?
		cout << "test BehaviorManager operator=" << endl;
		cout << &this->behaviorList << endl;
		cout << &this->behaviorList << endl;
		exit(0);
	}
	virtual ~BehaviorManager() {
		//	removes itself from all its Behaviors's list
		for (auto i : this->behaviorList) {
			i->removeTarget(this);
		}
		//	erase/empty list ?
	}
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
			find the object in the behavior targetList, and set to false the bool in the pair<void*, bool>
		--/>
			
			b2.removeTarget(o5);				//	remove the object from the behavior;
			delete b1;
		}


		*	comment savoir si l'object a un behavior si on ne stocke pas ce dernier ?
			-> std::list<Object>::iterator	it = std::find(list.begin(), list.end(), target);
			/!\ fastidieux, on doit donc le faire sur toutes les list des behaviors
				-> stocker le behavior dans l'object:	list<Behavior>	*beList;
				target.beList
		
	Behavior::
	bx	ox	bool
	bx	ox	bool
	bx	ox	bool
	bx	ox	bool
	bx	ox	bool
	bx	ox	bool
	sorted by b ?
	sorted by o ?

	b1			o1
	ox	bool	bx	bool
	ox	bool	bx	bool
	ox	bool	bx	bool
	ox	bool	bx	bool
	ox	bool	bx	bool

		o o o o o o o o o
	b	. . . . . . . . .
	b	. . . . . . . . .
	b	. . . . . . . . .
	b	. . . . . . . . .
	b	. . . . . . . . .
	b	. . . . . . . . .
*/