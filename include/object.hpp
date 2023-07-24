#pragma once
#include "math.hpp"
#include "properties.hpp"
#include "behaviormanaged.hpp"

class Properties;
// class BehaviorManaged;

class Object : public BehaviorManaged
{
public:
#if UNIT_TESTS == true
	friend class UnitTests;
#endif
	static unsigned int		getInstanceAmount();//make a class for that ?
	/*
		class Object : public InstanceManager, public BehaviorManager
		class OtherClass : public InstanceManager
		/!\ the static variables will be common to every derived, we dont want that
			-> search for a good way
	*/

	Object();
	Object(Properties object_pp);	// need ref ?
	Object(const Object& src);
	~Object();
	Object&	operator=(const Object& src);

	bool			update();
	//mutators
	// void			setMotionBehavior(/*...*/);//useless if _motionBehavior is public
	void			setParent(Object* parent);
	//accessors
	unsigned int	getId(void) const;
	Properties		getLocalProperties() const;
	Math::Matrix4&	getWorldMatrix() const;//todo:dangerous, check if used correctly
	Object*			getParent() const;

	//settings
	Properties		local;//is public, is that safe? we can do: obj.local = anotherLocal;
	bool			_worldMatrixChanged;
protected:

	unsigned int	_id;
	Object*			_parent;
	Math::Matrix4	_worldMatrix;
	// Properties		_world;//pp trop complique a recuperer depuis la matrice finale
	// Program&		_program;
private:
	//duplicate this in Obj (amount, Obj3d amount will be included in Obj amount)
	static unsigned int	_instanceAmount;	// should never be altered manually
	static unsigned int	_instanceId;		// should never be altered manually

	//this should not be used anymore, better send vectors of objects to the right renderer, and not iterate through every object
	virtual void	render(Math::Matrix4& PVmatrix);
};
