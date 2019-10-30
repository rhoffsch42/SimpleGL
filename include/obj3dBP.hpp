#pragma once
#include "simplegl.h"
#include "blueprint.hpp"
#include "math.hpp"
#include <iomanip>
#include <algorithm>

#define OBJ3DBP_DEFAULT_SIZE	4.0f

class Obj3dBP : public Blueprint
{
public:
	static float			defaultSize;

	Obj3dBP(string filename, bool rescale = true, bool center = true);
	Obj3dBP(Obj3dBP const & src);
	~Obj3dBP();
	Obj3dBP&	operator=(const Obj3dBP& obj3dbp);//a refaire

	//accessors
	int				getFaceAmount(void) const;
	Math::Vector3	getDimensions(void) const;
	bool			isCentered(void) const;
	bool			isRescaled(void) const;

protected:
private:
	// file data
	int				_faceAmount;
	Math::Vector3	_dimensions;
	bool			_centered;
	bool			_rescaled;
};

