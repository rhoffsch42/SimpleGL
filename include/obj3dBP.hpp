#pragma once
#include "simplegl.h"
#include "blueprint.hpp"
#include "math.hpp"
#include <iomanip>

class Obj3dBP : public Blueprint
{
public:
	Obj3dBP(string filename);//temporary, still ?
	Obj3dBP(Obj3dBP const & src);
	~Obj3dBP();
	Obj3dBP&	operator=(const Obj3dBP& obj3dbp);

	//accessors
	int				getFaceAmount(void) const;
	Math::Vector3	getCenterOffset(void) const;

protected:
private:
	// file data
	int				_faceAmount;
	Math::Vector3	_centerOffset;

};

