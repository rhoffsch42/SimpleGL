#pragma once
#include "simplegl.h"
#include "blueprint.hpp"
#include "math.hpp"
#include <iomanip>
#include <algorithm>
#include <vector>

#define OBJ3DBP_DEFAULT_SIZE	4.0f
#define BP_INDICES		0	// for glDrawElements
#define BP_VERTEX_ARRAY	1	// for glDrawArrays

/*
	This class assembles all meches of a Model.
*/
class Obj3dBP : public Blueprint
{
public:
	static float		defaultSize;//1
	static uint8_t		dataMode; //BP_INDICES
	static bool			rescale; //true
	static bool			center; //true


	Obj3dBP(string filename);
	Obj3dBP(Obj3dBP const & src);//private? or delete
	~Obj3dBP();
	Obj3dBP&	operator=(const Obj3dBP& obj3dbp);//a refaire

	//accessors
	uint8_t			getDataMode(void) const;
	const GLuint*	getIndicesData(void) const;
	GLuint			getVboVertex(void) const;
	GLuint			getVboColor(void) const;
	GLuint			getVboTexture(void) const;

	int				getFaceAmount(void) const;
	Math::Vector3	getDimensions(void) const;
	bool			isCentered(void) const;
	bool			isRescaled(void) const;

	int				elem_count;
protected:
private:
	uint8_t				_dataMode;
	std::vector<GLuint>	_indices;// for glDrawElements
	GLuint				_vboVertex;// for array vertex (glDrawArrays), or simple data vertex (glDrawElements)
	GLuint				_vboColor;// colors for each face
	GLuint				_vboTexture;//texture coo for each face
	// file data
	int				_faceAmount;
	Math::Vector3	_dimensions;
	bool			_centered;
	bool			_rescaled;
};

