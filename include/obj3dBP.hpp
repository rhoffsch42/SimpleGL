#pragma once
#include "simplegl.h"
#include "blueprint.hpp"
#include "math.hpp"

#include <iomanip>
#include <algorithm>
#include <vector>

#define OBJ3DBP_DEFAULT_SIZE	4.0f
#define BP_INDICES				0	// for glDrawElements
#define BP_LINEAR				1	// for glDrawArrays

#define BP_FREE_INDICES			0b00000001
#define BP_FREE_VERTICES		0b00000010
#define BP_FREE_ALL				0b11111111

#define BP_DONT_NORMALIZE		0b00000001

struct SimpleVector2 {
	SimpleVector2();
	SimpleVector2(float x, float y);
	float	x = 0.0f;
	float	y = 0.0f;
};

struct SimpleVertex {
	Math::Vector3	position;
	SimpleVector2	texCoord;
	Math::Vector3	color;
	//Math::Vector3	Normal;
	bool			operator<(const SimpleVertex& rhs) const {//to be used as functor
		return std::tie(position.x, position.y, position.z) < std::tie(rhs.position.x, rhs.position.y, rhs.position.z);
	}
};

struct TinyVertex {
	uint8_t	x;
	uint8_t	y;
	uint8_t	z;
	bool			operator<(const TinyVertex& rhs) const {//to be used as functor
		return std::tie(x, y, z) < std::tie(rhs.x, rhs.y, rhs.z);
	}
	//bool			operator==(const TinyVertex& rhs) const {//to be used as functor
	//	return (this->x != rhs.x && this->y != rhs.y && this->z != rhs.z);
	//}

};

class Obj3dBP;
class LodBP
{
public:
	LodBP() = delete;
	LodBP(Obj3dBP& bp, float min = -1.0f, float max = std::numeric_limits<float>::infinity());
	LodBP(const LodBP& bp);
	LodBP&	operator=(const LodBP& rhs);
	~LodBP();

	std::string	toString() const;

	Obj3dBP*	blueprint;
	float		minDistance = -1.0f;
	float		maxDistance = std::numeric_limits<float>::infinity();
};

class ConfigurationObj3dBP
{
public:
	ConfigurationObj3dBP();
	~ConfigurationObj3dBP();
	uint8_t		dataMode = BP_INDICES;
	float		modelSize = OBJ3DBP_DEFAULT_SIZE;
	bool		rescale = false;
	bool		center = true;
};

/*
	This class assembles all meches of a Model.
	the model must have only 1 texture for all meshes
*/
class Obj3dBP : public Blueprint
{
public:
	static ConfigurationObj3dBP	config;

	Obj3dBP(std::string filename);
	//array must be linear
	Obj3dBP(const std::vector<SimpleVertex>& src_vertices, const std::vector<unsigned int>& src_indices, unsigned int flags = 0);
	//merge everything into 1 BP, all BP must have the same _dataMode (BP_LINEAR or BP_INDICES)
	Obj3dBP(std::vector<Obj3dBP*> src, unsigned int flags = 0);

	Obj3dBP(Obj3dBP const& src) = delete;
	Obj3dBP	operator=(const Obj3dBP& rhs) = delete;
	~Obj3dBP();


	//accessors
	uint8_t						getDataMode(void) const;
	std::vector<GLuint>			getIndices(void) const;//lod
	std::vector<SimpleVertex>	getVertices(void) const;//lod
	GLuint						getVboVertex(void) const;
	GLuint						getEboIndices(void) const;
	//obsolete vbo
	//GLuint						getVboColor(void) const;
	//GLuint						getVboTexture(void) const;

	unsigned int				getPolygonAmount(void) const;//lod
	Math::Vector3				getDimensions(void) const;//generaly used for frustum culling
	bool						isCentered(void) const;
	bool						isRescaled(void) const;
	bool						isNormalized(void) const;

	void						freeData(unsigned int flags);//lods


protected:
private:
	uint8_t						_dataMode;
	std::vector<GLuint>			_indices;// for glDrawElements
	std::vector<SimpleVertex>	_vertices;
	GLuint						_eboIndices;// element array buffer object to store indices
	GLuint						_vboVertex;// for array vertex (glDrawArrays), or simple data vertex (glDrawElements)
	//obsolete vbo
	//GLuint				_vboColor;// colors for each face
	//GLuint				_vboTexture;//texture coo for each face

	// file data
	unsigned int	_polygonAmount;// triangles
	Math::Vector3	_dimensions;
	bool			_centered;
	bool			_rescaled;
	bool			_normalized;

	// dimensions and centering
	// todo: make a class for vertices management ? 
	void	normalize();
	//void	_normalizeVertices(std::vector<SimpleVertex>& vertices);

	void	loadToGPU();
};

