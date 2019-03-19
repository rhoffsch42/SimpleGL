#pragma once
#include <utility>

#include "object.hpp"
#include "behavior.hpp"
#include "math.hpp"
//include fps.h

#define MULTIPLICATIVE	false
#define ADDITIVE		true

//	The transformation is in local space

class TransformBH : public Behavior {
public:
	static bool areActive;

	TransformBH();
	TransformBH(t_pp transformValues);
	TransformBH(Math::Vector3 pos, Math::Rotation rot, Math::Vector3 scale);
	TransformBH(const TransformBH& src);
	~TransformBH();
	TransformBH&	operator=(const TransformBH& src);

	void	run();
	bool	isCompatible(BehaviorManager* target) const;
	/*
		target must have a local variable:
		class T {
		public:
			Properties	local;
		}
	*/

	//	true = additive, false = multiplicative (or absolute/relative)
	//	see properties.hpp for a better system (changing operator)
	t_pp	transform;
	bool	modePos;
	bool	modeRot;
	bool	modeScale;
};
