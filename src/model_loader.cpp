#include "model_loader.hpp"

//#define SGL_DEBUG
#ifdef SGL_DEBUG
#define SGL_MODEL_LOADER_DEBUG
#endif
#ifdef SGL_MODEL_LOADER_DEBUG 
#define D(x) std::cout << "[IModelLoader] " << x
#define D_(x) x
#define D_SPACER "-- model_loader.cpp -------------------------------------------------\n"
#define D_SPACER_END "----------------------------------------------------------------\n"
#else 
#define D(x)
#define D_(x)
#define D_SPACER ""
#define D_SPACER_END ""
#endif

IModelLoader::IModelLoader() {}
IModelLoader::~IModelLoader() {}

AssimpLoader::AssimpLoader() {}
AssimpLoader::~AssimpLoader() {}

void	AssimpLoader::loadFile(const std::string& path, std::vector<SimpleVertex>* dstVertices, std::vector<unsigned int>* dstIndices, unsigned int* polygonCount) {
	this->_vertices = dstVertices;
	this->_indices = dstIndices;

	// https://stackoverflow.com/questions/39269121/assimp-loader-with-a-cube-of-8-vertices
	Assimp::Importer importer;
	// http://assimp.sourceforge.net/lib_html/postprocess_8h.html
	unsigned int flags = aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_PreTransformVertices | aiProcess_OptimizeGraph;
	const aiScene* scene = importer.ReadFile(path, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		D("ERROR::ASSIMP::" << importer.GetErrorString() << std::endl);
		std::exit(2);
	}
	//string directory = path.substr(0, path.find_last_of("/\\"));
	this->_processNode(scene->mRootNode, scene);
	*polygonCount = this->_polygonCount;

	this->_polygonCount = 0;
	this->_vertices = nullptr;
	this->_vertices = nullptr;
}

SimpleVertex	AssimpLoader::_processVertexAt(aiMesh* mesh, const aiScene* scene, unsigned int indice) const {
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
	}
	else { vertex.texCoord = SimpleVector2(0.0f, 0.0f); }
	// color
	if (mesh->mColors[0]) {
		vertex.color.x = mesh->mColors[0][indice].r;
		vertex.color.y = mesh->mColors[0][indice].g;
		vertex.color.z = mesh->mColors[0][indice].b;
	}
	else if (mesh->mMaterialIndex >= 0) {
		const aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];
		aiColor4D diffuse;
		if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS) {
			vertex.color.x = diffuse.r;
			vertex.color.y = diffuse.g;
			vertex.color.z = diffuse.b;
		}
		else { vertex.color = Math::Vector3(200, 0, 200); }
	}
	else { vertex.color = Math::Vector3(200, 0, 200); }
	// normal
	//vector.x = mesh->mNormals[indice].x;
	//vector.y = mesh->mNormals[indice].y;
	//vector.z = mesh->mNormals[indice].z;
	//vertex.normal = vector;
	return vertex;
}

void	AssimpLoader::_processMesh(aiMesh* mesh, const aiScene* scene) {
	unsigned int loaded_vertex = this->_vertices->size();//save the amount of loaded vertex before this new mesh
	if (this->dataMode == BP_INDICES) {
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			this->_vertices->push_back(this->_processVertexAt(mesh, scene, i));
		}
	}
	else if (this->dataMode == BP_LINEAR) {
		//process with indices later
	}
	else {
		D(__PRETTY_FUNCTION__ << " : data corrupt:\n\twrong dataMode: " << (int)this->dataMode << std::endl);
		Misc::breakExit(23);
	}

	// process indices
	this->_polygonCount += mesh->mNumFaces;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			unsigned int indice = face.mIndices[j];
			if (this->dataMode == BP_LINEAR) {
				this->_vertices->push_back(this->_processVertexAt(mesh, scene, indice));
			}
			else if (this->dataMode == BP_INDICES) {
				this->_indices->push_back(indice + loaded_vertex);//as we merge all meshes, we have to take in account the already loaded ones
			}

		}
	}
}

void			AssimpLoader::_processNode(aiNode* node, const aiScene* scene) {
	unsigned int	polygonCount = 0;
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->_processMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		this->_processNode(node->mChildren[i], scene);
	}
}
