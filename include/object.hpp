#pragma once
#include "math.hpp"
#include "properties.hpp"

#define OBJ3D_DEFAULT_SIZE	4.0f

class Object
{
public:
	static unsigned int		getInstanceAmount();

	Object();
	Object(const Object& src);
	~Object();
	Object&	operator=(const Object& src);

	void	runMothionBehavior(void* ptr);
	void	update();
	void	render(Math::Matrix4& PVmatrix);//? virtual pure ?
	//mutators
	// void			setMotionBehavior(/*...*/);//useless if _motionBehavior is public

	//accessors
	unsigned int	getId(void) const;
	void			(*getMotionBehaviorFunc(void) const) (Object&, void*);
	Properties		getWorldProperties() const;
	Properties		getLocalProperties() const;

	//settings
	bool			_motionBehavior;
	//motion behavior
	void			(*_motionBehaviorFunc)(Object& ref, void* ptr);
	/*
		The static variables behavior must NOT depend on the object properties.
		As they do not belong to the Obj3d instance,
		its static variables will be altered by all instances using it.
	*/
protected:
private:
	//duplicate this in Obj (amount, Obj3d amount will be included in Obj amount)
	static unsigned int	_instanceAmount;	// should never be altered manually
	static unsigned int	_instanceId;		// should never be altered manually

	unsigned int	_id;
	Object*			_parent;// ref or pointer ? test whith ref, then delete original parent.
	Properties		_world;//public ?
	Properties		_local;//public ?
	// Program&		_program;
};
