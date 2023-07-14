#include "simplegl.h"
#include "obj3dBP.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"
#include "compiler_settings.h"
#include <algorithm>

#define SGL_DEBUG
#ifdef SGL_DEBUG
 #define SGL_OBJ3DBP_DEBUG
#endif
#ifdef SGL_OBJ3DBP_DEBUG 
 #define D(x) std::cout << "[Obj3dBP] " << x
 #define D_(x) x
 #define D_SPACER "-- obj3dBP.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

float			Obj3dBP::defaultSize = OBJ3DBP_DEFAULT_SIZE;
uint8_t			Obj3dBP::defaultDataMode = BP_INDICES;
bool			Obj3dBP::rescale = true;
bool			Obj3dBP::center = true;

static float	calcScaleCoef(Math::Vector3 dimensions, float size) {
	float	largest = dimensions.x;
	largest = std::max(largest, dimensions.y);
	largest = std::max(largest, dimensions.z);
	return (size / largest);
}

SimpleVertex	Obj3dBP::assimpProcessVertexAt(aiMesh* mesh, const aiScene* scene, unsigned int indice) const {
	// process vertex positions, texture coordinates, and colors
	SimpleVertex vertex;

	// positions
	vertex.position.x = mesh->mVertices[indice].x;
	vertex.position.y = mesh->mVertices[indice].y;
	vertex.position.z = mesh->mVertices[indice].z;
	// texture UV
	if (mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
		vertex.texCoord.x = mesh->mTextureCoords[0][indice].x;
		vertex.texCoord.y = mesh->mTextureCoords[0][indice].y;
	} else { vertex.texCoord = SimpleVector2(0.0f, 0.0f); }
	// color
	if (mesh->mColors[0]) {
		vertex.color.x = mesh->mColors[0][indice].r;
		vertex.color.y = mesh->mColors[0][indice].g;
		vertex.color.z = mesh->mColors[0][indice].b;
	} else if (mesh->mMaterialIndex >= 0) {
		const aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];
		aiColor4D diffuse;
		if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS) {
			vertex.color.x = diffuse.r;
			vertex.color.y = diffuse.g;
			vertex.color.z = diffuse.b;
		} else { vertex.color = Math::Vector3(200, 0, 200); }
	} else { vertex.color = Math::Vector3(200, 0, 200); }
	// normal
	//vector.x = mesh->mNormals[indice].x;
	//vector.y = mesh->mNormals[indice].y;
	//vector.z = mesh->mNormals[indice].z;
	//vertex.normal = vector;
	return vertex;
}

void	Obj3dBP::assimpProcessMesh(aiMesh* mesh, const aiScene* scene) {

	unsigned int loaded_vertex = this->_vertices.size();//save the amount of loaded vertex before this new mesh
	if (this->_dataMode == BP_INDICES) {
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			this->_vertices.push_back(this->assimpProcessVertexAt(mesh, scene, i));
		}
	} else if (this->_dataMode == BP_LINEAR) {
		//process with indices later
	} else {
		D(__PRETTY_FUNCTION__ << " : data corrupt:\n\twrong dataMode: " << (int)this->_dataMode << std::endl);
		Misc::breakExit(23);
	}

	// process indices
	this->_polygonAmount += mesh->mNumFaces;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			unsigned int indice = face.mIndices[j];
			if (this->_dataMode == BP_LINEAR) {
				this->_vertices.push_back(this->assimpProcessVertexAt(mesh, scene, indice));
			} else if (this->_dataMode == BP_INDICES) {
				this->_indices.push_back(indice + loaded_vertex);//as we merge all meshes, we have to take in account the already loaded ones
			}

		}
	}
}

void	Obj3dBP::assimpProcessNode(aiNode* node, const aiScene* scene) {
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->assimpProcessMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		this->assimpProcessNode(node->mChildren[i], scene);
	}
}

void	Obj3dBP::loadWithAssimp(std::string path) {
	D(__PRETTY_FUNCTION__ << std::endl);
	//https://stackoverflow.com/questions/39269121/assimp-loader-with-a-cube-of-8-vertices
	Assimp::Importer importer;
	//http://assimp.sourceforge.net/lib_html/postprocess_8h.html
	unsigned int flags = aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_PreTransformVertices | aiProcess_OptimizeGraph;
	const aiScene* scene = importer.ReadFile(path, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		D("ERROR::ASSIMP::" << importer.GetErrorString() << std::endl);
		std::exit(2);
	}
	//string directory = path.substr(0, path.find_last_of("/\\"));
	this->assimpProcessNode(scene->mRootNode, scene);

	D(__PRETTY_FUNCTION__ << " END" << std::endl);
}

Obj3dBP::Obj3dBP(std::string filename) : Blueprint(filename) {
	D("Obj3dBP::Obj3dBP(std::string filename)\n");
	filename = Misc::crossPlatPath(filename);

	this->_dataMode = Obj3dBP::defaultDataMode;
	this->_polygonAmount = 0;
	this->_eboIndices = 0;
	this->_vboVertex = 0;
	this->_centered = Obj3dBP::center;
	this->_rescaled = Obj3dBP::rescale;
	this->loadWithAssimp(filename);
	this->normalize();
	//std::stringstream ss;
	//for (auto i : this->_indices)
	//	ss << i << " ";
	//ss << "\n\n";
	//for (auto i : vertices)
	//	ss << i.position.toString();
	//ss << "\n\n";
	//Misc::breakExit(0);
	this->loadToGPU();

	D("vertices:\t" << this->_vertices.size() << std::endl);
	D("indices: \t" << this->_indices.size() << std::endl);

	D(__PRETTY_FUNCTION__ << " END" << std::endl);
	D(D_SPACER_END << std::endl);
}

Obj3dBP::Obj3dBP(const std::vector<SimpleVertex>& src_vertices, const std::vector<unsigned int>& src_indices, unsigned int flags) : Blueprint("N/A by vertices") {
	D("Obj3dBP::Obj3dBP(vertices, indices)\n");
	this->_eboIndices = 0;
	this->_vboVertex = 0;
	this->_centered = false;
	this->_rescaled = false;
	this->_normalized = false;
	this->_vertices = src_vertices;//todo: std::move?
	if (src_indices.empty()) {
		this->_dataMode = BP_LINEAR;
		this->_polygonAmount = src_vertices.size() / 3;
	}
	else {
		this->_dataMode = BP_INDICES;
		this->_indices = src_indices;//todo: std::move?
		this->_polygonAmount = src_indices.size() / 3;
	}
	D((this->_dataMode == BP_LINEAR ? "BP_LINEAR\n" : "BP_INDICES\n"));
	if ((flags & BP_DONT_NORMALIZE) != BP_DONT_NORMALIZE) {
		D("Normalizing... ");
		this->normalize();
		D("Done.\n");
	}
	D("Init buffers... ");
	this->loadToGPU();
	D("Done.\n");
	D("_ end Obj3dBP::Obj3dBP(vertices, indices)\n");
}

/*
	This will merge multiple objects BPs in a single object BP (they probably will be intricated).
*/
Obj3dBP::Obj3dBP(std::vector<Obj3dBP*> src, unsigned int flags) : Blueprint("N/A by BPs merging") {
	this->_polygonAmount = 0;
	this->_eboIndices = 0;
	this->_vboVertex = 0;
	this->_centered = false;
	this->_rescaled = false;
	this->_normalized = false;

	this->_dataMode = src.front()->getDataMode();
	for (auto bp : src) {
		if (bp->getDataMode() != this->_dataMode) {
			D("Obj3dBP merging constructor error: a BP has a different data mode.\n");
			Misc::breakExit(56);
		}
		const std::vector<SimpleVertex>& vert = bp->getVertices();
		this->_vertices.insert(this->_vertices.end(), vert.begin(), vert.end());
		if (this->_dataMode == BP_INDICES) {
			std::vector<GLuint>& ind = bp->getIndices();
			this->_indices.insert(this->_indices.end(), ind.begin(), ind.end());
		}
		this->_polygonAmount += bp->getPolygonAmount();
	}
	if (this->_polygonAmount != this->_vertices.size() / 3) {
		D("Obj3dBP merging constructor error: data corrupted when merging polygons.\n");
		D(this->_polygonAmount << " != " << (this->_vertices.size() / 3) << "\n");
		Misc::breakExit(56);
	}

	if ((flags & BP_DONT_NORMALIZE) != BP_DONT_NORMALIZE)
		this->normalize();
	this->loadToGPU();
}

Obj3dBP::Obj3dBP(const Obj3dBP& src) : Blueprint(src) {
	D("Obj3dBP cons by copy" << std::endl);
	D("building object: " << src.getName().c_str() << std::endl);

	*this = src;
}

Obj3dBP::~Obj3dBP() {
	D(__PRETTY_FUNCTION__ << "\n");
	D("vertices:\t" << this->_vertices.size() << std::endl);
	D("indices: \t" << this->_indices.size() << std::endl);
	this->_vertices.clear();
	this->_indices.clear();

	if (this->_dataMode == BP_INDICES) {
		glDeleteBuffers(1, &this->_eboIndices);
	}
	glDeleteBuffers(1, &this->_vboVertex);
	glDeleteVertexArrays(1, &this->_vao);
}

Obj3dBP& Obj3dBP::operator=(const Obj3dBP& src) {
	D("Obj3dBP operator =" << std::endl);
	//what do we do for vbo? see .hpp
	this->_dataMode = src._dataMode;
	this->_indices = src._indices;
	this->_polygonAmount = src._polygonAmount;
	this->_dimensions = src._dimensions;
	this->_centered = src._centered;
	this->_rescaled = src._rescaled;
	return (*this);
}

//mutators
//accessors
uint8_t						Obj3dBP::getDataMode(void) const { return this->_dataMode; }
std::vector<GLuint>			Obj3dBP::getIndices(void) const { return this->_indices; }
std::vector<SimpleVertex>	Obj3dBP::getVertices(void) const { return this->_vertices; }
GLuint						Obj3dBP::getVboVertex(void) const { return this->_vboVertex; }
GLuint						Obj3dBP::getEboIndices(void) const { return this->_eboIndices; }
//GLuint						Obj3dBP::getVboColor(void) const { return this->_vboColor; }
//GLuint						Obj3dBP::getVboTexture(void) const { return this->_vboTexture; }
int							Obj3dBP::getPolygonAmount(void) const { return this->_polygonAmount; }
Math::Vector3				Obj3dBP::getDimensions(void) const { return this->_dimensions; }
bool						Obj3dBP::isCentered(void) const { return this->_centered; }
bool						Obj3dBP::isRescaled(void) const { return this->_rescaled; }
bool						Obj3dBP::isNormalized(void) const { return this->_normalized; }

void			Obj3dBP::freeData(unsigned int flags) {
	if ((flags & BP_FREE_INDICES) == BP_FREE_INDICES) {
		this->_indices.clear();
	}
	if ((flags & BP_FREE_VERTICES) == BP_FREE_VERTICES) {
		this->_vertices.clear();
	}
}

/*
	is it really normalized if centered and rescaled are false?
*/
#define NORMALIZE_WARNING_THRESHOLD	500000
void	Obj3dBP::normalize() {
	// dimensions, vertices rescale and centering
	this->_normalized = true;
	//if (!this->_vertices.size()) {return;}

	if (this->_vertices.size() >= NORMALIZE_WARNING_THRESHOLD) {
		D("Warning: normalizing a big Obj3dBP(" << NORMALIZE_WARNING_THRESHOLD << "+ vertices), performances might be impacted.\n");
	}

	float	vmin[3];
	float	vmax[3];
	vmin[0] = this->_vertices[0].position.x;
	vmin[1] = this->_vertices[1].position.x;
	vmin[2] = this->_vertices[2].position.x;
	vmax[0] = vmin[0];
	vmax[1] = vmin[1];
	vmax[2] = vmin[2];

	// finding each min and max of x, y and z
	for (size_t i = 0; i < this->_vertices.size(); i++) {
		float vx = this->_vertices[i].position.x;
		float vy = this->_vertices[i].position.y;
		float vz = this->_vertices[i].position.z;
		vmin[0] = (vx < vmin[0]) ? vx : vmin[0];
		vmin[1] = (vy < vmin[1]) ? vy : vmin[1];
		vmin[2] = (vz < vmin[2]) ? vz : vmin[2];
		vmax[0] = (vx > vmax[0]) ? vx : vmax[0];
		vmax[1] = (vy > vmax[1]) ? vy : vmax[1];
		vmax[2] = (vz > vmax[2]) ? vz : vmax[2];

	}
	// calc the dimension
	this->_dimensions.x = vmax[0] - vmin[0];
	this->_dimensions.y = vmax[1] - vmin[1];
	this->_dimensions.z = vmax[2] - vmin[2];

	// centering 
	Math::Vector3	centerOffset;
	centerOffset.x = (vmin[0] + vmax[0]) / 2;
	centerOffset.y = (vmin[1] + vmax[1]) / 2;
	centerOffset.z = (vmin[2] + vmax[2]) / 2;
	if (this->_centered) {
		for (size_t i = 0; i < this->_vertices.size(); i++) {
			this->_vertices[i].position.x -= centerOffset.x;
			this->_vertices[i].position.y -= centerOffset.y;
			this->_vertices[i].position.z -= centerOffset.z;
		}
	}

	float	scaleCoef = 1.0f;
	if (this->_rescaled) {
		scaleCoef = calcScaleCoef(this->_dimensions, Obj3dBP::defaultSize);
		this->_dimensions.mult(scaleCoef);
		for (size_t i = 0; i < this->_vertices.size(); i++) {
			this->_vertices[i].position.mult(scaleCoef);
		}
	}

#if 0
	D("polygons { " << this->_polygonAmount << " }" << std::endl);
	D("vertices array size { " << this->_vertices.size() << " }" << std::endl);
	D("indices array size { " << this->_indices.size() << " }" << std::endl);
	D((this->_centered ? "YES" : "NO"));
	D(" center offset:\t" << centerOffset.x << " " << centerOffset.y << " " << centerOffset.z << std::endl);
	D("scale coef:\t" << scaleCoef << std::endl);
#endif
	if (LOGFILES) {
		std::stringstream logs;
		int c1 = 0;
		logs << std::endl;
		logs << "total polygons:\t" << c1 << std::endl;
		logs << "polygons { " << this->_polygonAmount << " }" << std::endl;
		logs << "rescaled: " << (this->_rescaled ? "yes" : "no") << std::endl;
		logs << "scale coef:\t" << scaleCoef << std::endl;
		logs << "centered: " << (this->_centered ? "yes" : "no") << std::endl;
		logs << "center offset:\t" << centerOffset.x << " " << centerOffset.y << " " << centerOffset.z << std::endl;

		//data
		logs << "vertices:\t";
		for (size_t i = 0; i < this->_vertices.size(); i++) {
			logs << this->_vertices[i].position.x << " ";
			logs << this->_vertices[i].position.y << " ";
			logs << this->_vertices[i].position.z << ", ";
		}
		logs << std::endl << "size: " << this->_vertices.size() << " : " << this->_vertices.size() << std::endl;
		Misc::logfile(std::string("Obj3dBP_vao_") + std::to_string(this->_vao), logs.str());
	}

}

/*
	loading blueprint with opengl, init buffers
*/
void	Obj3dBP::loadToGPU() {
	//VAO
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);

	/*
		these buffers are created in a context that can potentially be shared with some other contexts 
	*/

	// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
	// EBO glDrawElements
	if (this->_dataMode == BP_INDICES) {
		if (0) {//display data
			for (size_t i = 0; i < this->_indices.size(); i++) {
				D(this->_indices[i] << " ");
			}D(std::endl);
			for (size_t i = 0; i < this->_vertices.size(); i++) {
				D(this->_vertices[i].position.toString() << "\n");
				D(" ");
			}D(std::endl);
		}
		glGenBuffers(1, &this->_eboIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_eboIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indices.size() * sizeof(unsigned int), this->_indices.data(), GL_STATIC_DRAW);
	}
	//VBO
	glGenBuffers(1, &this->_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, this->_vertices.size() * sizeof(SimpleVertex), this->_vertices.data(), GL_STATIC_DRAW);//1 vbo for everything

	glBindVertexArray(0);
}

/*

2500 Models * 10252 polygons = 25 630 000 polygons
PC-1      Intel Core i7 2600 @ 3.40GHz Sandy Bridge 32nm Technology    4095MB NVIDIA GeForce GTX 1050 Ti (MSI)
PC-2      Intel Core i7 @ 3.70GHz Coffee Lake 14nm Technology          4095MB NVIDIA GeForce GTX 1080
Shadow15  Intel Xeon E5 v3 @ 3.20GHz Haswell-E/EP 22nm Technology      3071MB NVIDIA Quadro P5000 (NVIDIA)

                                           FPS
--------------------------+-----------+-----------+-----------|
                     PC   |  PC-1     |  Shadow15 |  PC-2     |
                frustum   |  in  out  |  in  out  |  in  out  |
--------------------------+-----------+-----------+-----------|
    loaded with TINYOBJ manual build  18573/3 = 6191  vertices for 30756 indices
                        linear array  92268/3 = 30756 vertices
--------------------------+-----------+-----------+-----------|
glDrawArrays              |  25   37  | 112  112  | 126  132  |
glDrawArraysInstanced     |  25   33  |  98  102  | 102  110  |
--------------------------+-----------+-----------+-----------|
glDrawElements            |  22   22  |  33   33  |  29   29  |
glDrawElementsInstanced   |  36   40  |  40   40  |  58   60  |
--------------------------+-----------+-----------+-----------|
glDrawElements *          |  37   75  |  147  147 |           |
glDrawElementsInstanced * |  37   75  |  195  230 |           |
--------------------------+-----------+-----------+-----------|
    loaded and build with ASSIMP x64  21698/3 = 7232,6.. vertices for 30756 indices
--------------------------+-----------+-----------+-----------|
glDrawElements *          |  32   52  | 155   165 |           |
glDrawElementsInstanced * |  32   48  | 146   136 |           |
--------------------------+-----------+-----------+-----------|
* with EBO: nomore CPU -> GPU transfert of indices each frame

*/