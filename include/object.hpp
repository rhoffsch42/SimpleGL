#pragma once
#include "math.hpp"
#include "properties.hpp"
#include "behavior.hpp"

class Properties;
class BehaviorManager;

class Object : public BehaviorManager
{
public:
	static unsigned int		getInstanceAmount();//make a class for that ?
	/*
		class Object : public InstanceManager, public BehaviorManager
		class OtherClass : public InstanceManager
		/!\ the static variables will be common to every derived, we dont want that
			-> search for a good way
	*/

	Object();
	Object(Properties object_pp);
	Object(const Object& src);
	~Object();
	Object&	operator=(const Object& src);

	bool			update();
	virtual void	render(Math::Matrix4& PVmatrix);
	//mutators
	// void			setMotionBehavior(/*...*/);//useless if _motionBehavior is public
	void			setParent(Object* parent);
	//accessors
	unsigned int	getId(void) const;
	Properties		getLocalProperties() const;
	Math::Matrix4&	getWorldMatrix() const;//dangerous
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

};
