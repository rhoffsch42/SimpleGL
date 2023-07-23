#pragma once

#include "obj3dBP.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

// ie Obj3dBP loader
class IModelLoader
{
public:
	IModelLoader(const IModelLoader& src) = delete;
	IModelLoader& operator=(const IModelLoader& rhs) = delete;

	virtual void	loadFile(const std::string& path, std::vector<SimpleVertex>* dstVertices, std::vector<unsigned int> *dstIndices, unsigned int* polygonCount) = 0;
	uint8_t	dataMode = BP_LINEAR; // BP_LINEAR or BP_INDICES
protected:
	IModelLoader();
	virtual ~IModelLoader();
private:
};

class AssimpLoader : public IModelLoader
{
public:
	AssimpLoader();
	~AssimpLoader();

	virtual void	loadFile(const std::string& path, std::vector<SimpleVertex>* dstVertices, std::vector<unsigned int>* dstIndices, unsigned int* polygonCount);
private:
	SimpleVertex	_processVertexAt(aiMesh* mesh, const aiScene* scene, unsigned int indice) const;
	// returns polygon count
	void			_processMesh(aiMesh* mesh, const aiScene* scene);
	void			_processNode(aiNode* node, const aiScene* scene);

	std::vector<SimpleVertex>*	_vertices = nullptr;
	std::vector<unsigned int>*	_indices = nullptr;
	unsigned int				_polygonCount = 0;
};