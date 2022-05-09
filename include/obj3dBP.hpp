#pragma once
#include "simplegl.h"
#include "blueprint.hpp"
#include "math.hpp"
#include <iomanip>
#include <algorithm>
#include <vector>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define OBJ3DBP_DEFAULT_SIZE	4.0f
#define BP_INDICES		0	// for glDrawElements
#define BP_LINEAR		1	// for glDrawArrays

#define BP_FREE_INDICES		1
#define BP_FREE_VERTICES	2
#define BP_FREE_ALL			0xffffffff

#define BP_DONT_NORMALIZE		0x00000001

struct SimpleVector2 {
	SimpleVector2() : x(0), y(0) {}
	SimpleVector2(float x, float y) : x(x), y(y) {}
	float	x;
	float	y;
};

/*
	v  |   grid_size   |


	textures: 1 big texture containing 256 sprites (16x16 grid)

	https://www.youtube.com/watch?v=bGN445_2NSw
	chunk 32x32x32 :
		3 x 11bits per pos
		6 possible normals: 3bits



	buildVertexArrayFromOctree in GPU:
		1 voxel id containing:
			textureID,
				2 texCoo per 1 vertex computed on GPU
		neighbors_flag: determining if each faces are drawn
		voxel pos & size: allowing to compute each vertex for drawn faces
*/
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

/*
	This class assembles all meches of a Model.
	the model musst have only 1 texture for all meshes
*/
class Obj3dBP : public Blueprint
{
public:
	static float		defaultSize;//1
	static uint8_t		defaultDataMode; //BP_INDICES
	static bool			rescale; //true
	static bool			center; //true

	Obj3dBP(std::string filename);
	Obj3dBP(Obj3dBP const & src);//private? or delete

	//array must be linear
	Obj3dBP(const std::vector<SimpleVertex>& src_vertices, const std::vector<unsigned int>& src_indices, unsigned int flags = 0);

	//merge everything into 1 BP, all BP must have the same _dataMode (BP_LINEAR or BP_INDICES)
	Obj3dBP(std::vector<Obj3dBP*> src, unsigned int flags = 0);

	~Obj3dBP();
	Obj3dBP&	operator=(const Obj3dBP& obj3dbp);//a refaire

	//accessors
	uint8_t						getDataMode(void) const;
	std::vector<GLuint>			getIndices(void) const;
	std::vector<SimpleVertex>	getVertices(void) const;
	GLuint						getVboVertex(void) const;
	GLuint						getEboIndices(void) const;
	//obsolete vbo
	//GLuint						getVboColor(void) const;
	//GLuint						getVboTexture(void) const;

	int							getPolygonAmount(void) const;
	Math::Vector3				getDimensions(void) const;
	bool						isCentered(void) const;
	bool						isRescaled(void) const;
	bool						isNormalized(void) const;

	void						freeData(unsigned int flags);

protected:
private:
	uint8_t						_dataMode;
	std::vector<GLuint>			_indices;// for glDrawElements
	std::vector<SimpleVertex>	_vertices;
	GLuint						_eboIndices;//element array buffer object to store induces
	GLuint						_vboVertex;// for array vertex (glDrawArrays), or simple data vertex (glDrawElements)
	//obsolete vbo
	//GLuint				_vboColor;// colors for each face
	//GLuint				_vboTexture;//texture coo for each face
	// file data
	int				_polygonAmount;// triangles
	Math::Vector3	_dimensions;
	bool			_centered;
	bool			_rescaled;
	bool			_normalized;

	//loaders
	void			loadWithAssimp(std::string path);
	SimpleVertex	assimpProcessVertexAt(aiMesh* mesh, const aiScene* scene, unsigned int indice) const;
	void			assimpProcessMesh(aiMesh* mesh, const aiScene* scene);
	void			assimpProcessNode(aiNode* node, const aiScene* scene);

	// dimensions and centering
	void	normalize();
	void	initBuffers();
};

