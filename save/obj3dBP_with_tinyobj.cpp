#include "simplegl.h"
#include "obj3dBP.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"


float			Obj3dBP::defaultSize = OBJ3DBP_DEFAULT_SIZE;
uint8_t			Obj3dBP::defaultDataMode = BP_INDICES;
bool			Obj3dBP::rescale = true;
bool			Obj3dBP::center = true;

#define mymax(a, b) (((a) > (b)) ? (a) : (b))
#define mymin(a, b) (((a) < (b)) ? (a) : (b))

static float	calcScaleCoef(Math::Vector3 dimensions, float size) {
	float	largest = dimensions.x;
	largest = mymax(largest, dimensions.y);
	largest = mymax(largest, dimensions.z);
	return (size / largest);
}

void	Obj3dBP::loadWithTinyobj(string path) {
	std::string		cwd(Misc::getCurrentDirectory());
	size_t	i = path.find_last_of("/\\");
	if (i != std::string::npos) {
		change_cwd(path.substr(0, i).c_str());//change to the obj directory to be able to load other files like .mtl
		path = path.substr(i + 1);
	}

	// tinyobjloader loading 
	tinyobj::attrib_t					attrib;
	std::vector<tinyobj::shape_t>		shapes;
	std::vector<tinyobj::material_t>	materials;
	std::string		err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());
	if (!err.empty())
		cerr << err << endl;
	if (!ret) {
		cerr << "tinyobj: failed to load " << path << endl;
		Misc::breakExit(1);
	}
	if (i != std::string::npos)
		change_cwd(cwd.c_str());

	std::stringstream logs;
	int c1 = 0;
	if (LOGFILES) {
		logs << std::fixed << std::setprecision(2);

		logs << path << endl;
		logs << "# shapes:\t" << shapes.size() << endl;
		logs << "# vertices:\t " << attrib.vertices.size() << endl;

		logs << "# materials: " << materials.size() << endl;
		for (size_t i = 0; i < materials.size(); i++) {
			logs << materials[i].diffuse[0] << " ";
			logs << materials[i].diffuse[1] << " ";
			logs << materials[i].diffuse[2] << endl;
		}
	}

	//building blueprint
	/*
	typedef struct {
		std::vector<real_t> vertices;   // 'v'
		std::vector<real_t> normals;    // 'vn'
		std::vector<real_t> texcoords;  // 'vt'
		std::vector<real_t> colors;     // extension: vertex colors
	} attrib_t;
*/
	std::vector<SimpleVertex>	vertex_data;//build 1 vbo for position texcoords colors
	tinyobj::attrib_t* attrib_used = nullptr;
	float	vmin[3];
	float	vmax[3];
	vmin[0] = attrib.vertices[0];
	vmin[1] = attrib.vertices[1];
	vmin[2] = attrib.vertices[2];
	vmax[0] = vmin[0];
	vmax[1] = vmin[1];
	vmax[2] = vmin[2];


	tinyobj::attrib_t	attrib_linear;
	for (size_t s = 0; s < shapes.size(); s++) {// Loop over shapes

		if (LOGFILES) {
			logs << "group:\t" << shapes[s].mesh.num_face_vertices.size() << " polygons" << endl;
			c1 += shapes[s].mesh.num_face_vertices.size();
		}

		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {// Loop over faces(polygon)
			int fv = shapes[s].mesh.num_face_vertices[f];
			if (fv != 3) {
				cerr << path << " : bad obj format, polygons must be triangles only" << endl;
				Misc::breakExit(2);
			}

			for (int v = 0; v < fv; v++) {// Loop over vertices in the face.
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				SimpleVertex	vertex;
				// positions
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				attrib_linear.vertices.push_back(vx);
				attrib_linear.vertices.push_back(vy);
				attrib_linear.vertices.push_back(vz);
				vertex.position.x = vx;
				vertex.position.y = vy;
				vertex.position.z = vz;

				// normals
				tinyobj::real_t nx = 0;
				tinyobj::real_t ny = 0;
				tinyobj::real_t nz = 0;
				if (attrib.normals.size()) {
					nx = attrib.normals[3 * idx.normal_index + 0];
					ny = attrib.normals[3 * idx.normal_index + 1];
					nz = attrib.normals[3 * idx.normal_index + 2];
					attrib_linear.normals.push_back(nx);
					attrib_linear.normals.push_back(ny);
					attrib_linear.normals.push_back(nz);
				}
				//not now
				//vertex.normal.x = nx;
				//vertex.normal.y = ny;
				//vertex.normal.z = nz;

				// textures
				tinyobj::real_t tx = 0;
				tinyobj::real_t ty = 0;
				if (attrib.texcoords.size()) {
					tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					attrib_linear.texcoords.push_back(tx);
					attrib_linear.texcoords.push_back(ty);
				}
				vertex.texCoord.x = tx;
				vertex.texCoord.y = ty;

				// colors
				tinyobj::real_t r = 0;
				tinyobj::real_t g = 0;
				tinyobj::real_t b = 0;
				if (attrib.colors.size()) {
					int x = shapes[s].mesh.material_ids[f];
					r = materials[x].diffuse[0];
					g = materials[x].diffuse[1];
					b = materials[x].diffuse[2];
					attrib_linear.colors.push_back(r);
					attrib_linear.colors.push_back(g);
					attrib_linear.colors.push_back(b);
				}
				vertex.color.x = vx;
				vertex.color.y = vy;
				vertex.color.z = vz;

				vertex_data.push_back(vertex);

			}
			//	cout << endl;
			index_offset += fv;
			//shapes[s].mesh.material_ids[f];// per-face material
		}
	}

	//indices
	for (size_t s = 0; s < shapes.size(); s++) {// Loop over shapes
		for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {
			this->_indices.push_back(shapes[s].mesh.indices[i].vertex_index);
		}
	}

	this->_polygonAmount = (int)attrib_linear.vertices.size() / 9;//9 = xyz * 3
	if (this->_dataMode == BP_LINEAR) {
		attrib_used = &attrib_linear;
	} else if (this->_dataMode == BP_INDICES) {
		attrib_used = &attrib;
	} else {
		std::cout << __PRETTY_FUNCTION__ << " : data corrupt:\n\twrong dataMode: " << (int)this->_dataMode << std::endl;
		Misc::breakExit(23);
	}

	// dimensions and centering
	for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
		tinyobj::real_t vx = attrib.vertices[i + 0];
		tinyobj::real_t vy = attrib.vertices[i + 1];
		tinyobj::real_t vz = attrib.vertices[i + 2];
		vmin[0] = (vx < vmin[0]) ? vx : vmin[0];
		vmin[1] = (vy < vmin[1]) ? vy : vmin[1];
		vmin[2] = (vz < vmin[2]) ? vz : vmin[2];
		vmax[0] = (vx > vmax[0]) ? vx : vmax[0];
		vmax[1] = (vy > vmax[1]) ? vy : vmax[1];
		vmax[2] = (vz > vmax[2]) ? vz : vmax[2];

	}
	this->_dimensions.x = vmax[0] - vmin[0];
	this->_dimensions.y = vmax[1] - vmin[1];
	this->_dimensions.z = vmax[2] - vmin[2];

	Math::Vector3	centerOffset;
	centerOffset.x = (vmin[0] + vmax[0]) / 2;
	centerOffset.y = (vmin[1] + vmax[1]) / 2;
	centerOffset.z = (vmin[2] + vmax[2]) / 2;
	if ((this->_centered = Obj3dBP::center)) {
		for (size_t i = 0; i < attrib_used->vertices.size(); i += 3) {
			attrib_used->vertices[i + 0] -= centerOffset.x;
			attrib_used->vertices[i + 1] -= centerOffset.y;
			attrib_used->vertices[i + 2] -= centerOffset.z;
		}
		for (size_t i = 0; i < vertex_data.size(); i++) {
			vertex_data[i].position.x -= centerOffset.x;
			vertex_data[i].position.y -= centerOffset.y;
			vertex_data[i].position.z -= centerOffset.z;
		}
	}

	float	scaleCoef = 1.0f;
	if ((this->_rescaled = Obj3dBP::rescale)) {
		scaleCoef = calcScaleCoef(this->_dimensions, Obj3dBP::defaultSize);
		this->_dimensions.mult(scaleCoef);
		for (size_t i = 0; i < attrib_used->vertices.size(); i++) {
			attrib_used->vertices[i] *= scaleCoef;
		}
		// 3 positions + 2 texcoords + 3 colors = 8
		for (size_t i = 0; i < vertex_data.size(); i++) {
			vertex_data[i].position.mult(scaleCoef);
		}
	}

	std::cout << "polygons { " << this->_polygonAmount << " }" << endl;
	std::cout << "vertices array size { " << vertex_data.size() << " }" << endl;
	std::cout << "indices array size { " << this->_indices.size() << " }" << endl;
	std::cout << "center offset:\t" << centerOffset.x << " " << centerOffset.y << " " << centerOffset.z << endl;
	std::cout << "scale coef:\t" << scaleCoef << endl;
	if (LOGFILES) {
		logs << endl;
		logs << "total polygons:\t" << c1 << endl;
		logs << "polygons { " << this->_polygonAmount << " }" << endl;
		logs << "color { " << (int)attrib_used->colors.size() / 9 << " }" << endl;
		logs << "rescaled: " << (this->_rescaled ? "yes" : "no") << endl;
		logs << "scale coef:\t" << scaleCoef << endl;
		logs << "centered: " << (this->_centered ? "yes" : "no") << endl;
		logs << "center offset:\t" << centerOffset.x << " " << centerOffset.y << " " << centerOffset.z << endl;

		//data
		logs << "vertices:\t";
		for (size_t i = 0; i < attrib.vertices.size(); i++) {
			logs << attrib.vertices[i] << " ";
		}
		logs << endl << "size: " << attrib.vertices.size() << " : " << attrib.vertices.size() / 3 << endl;
		logs << "colors:\t";
		for (size_t i = 0; i < attrib.colors.size(); i++) {
			logs << attrib.colors[i] << " ";
		}
		logs << endl;

		Misc::logfile(string("Obj3dBP_" + path), logs.str());
	}

	//loading blueprint with opengl
	//VAO
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	//https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
	// glDrawElements
	if (this->_dataMode == BP_INDICES) {
		glGenBuffers(1, &this->_eboIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_eboIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indices.size() * sizeof(float), this->_indices.data(), GL_STATIC_DRAW);
	}

	//VBO position
	glGenBuffers(1, &this->_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboVertex);
	//glBufferData(GL_ARRAY_BUFFER, attrib_used->vertices.size() * sizeof(float), attrib_used->vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(SimpleVertex), vertex_data.data(), GL_STATIC_DRAW);//1 vbo for everything

	if (0) {
		//VBO color
		if (attrib_used->colors.size()) {
			glGenBuffers(1, &this->_vboColor);
			glBindBuffer(GL_ARRAY_BUFFER, this->_vboColor);
			glBufferData(GL_ARRAY_BUFFER, attrib_used->colors.size() * sizeof(float), attrib_used->colors.data(), GL_STATIC_DRAW);
		}
		//VBO texture
		if (attrib_used->texcoords.size()) {
			glGenBuffers(1, &this->_vboTexture);
			glBindBuffer(GL_ARRAY_BUFFER, this->_vboTexture);
			glBufferData(GL_ARRAY_BUFFER, attrib_used->texcoords.size() * sizeof(float), attrib_used->texcoords.data(), GL_STATIC_DRAW);
		}
		//VBO normals (todo)
		// ...
	}

	//end
	glBindBuffer(GL_ARRAY_BUFFER, 0);//attribution will be done later with a program
	glBindVertexArray(0);
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

void	Obj3dBP::assimpProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<SimpleVertex>& vertices) {

	unsigned int loaded_vertex = vertices.size();
	if (this->_dataMode == BP_INDICES) {
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			vertices.push_back(this->assimpProcessVertexAt(mesh, scene, i));
		}
	} else if (this->_dataMode == BP_LINEAR) {
		//process with indices later
	} else {
		std::cout << __PRETTY_FUNCTION__ << " : data corrupt:\n\twrong dataMode: " << (int)this->_dataMode << std::endl;
		Misc::breakExit(23);
	}

	// process indices
	this->_polygonAmount += mesh->mNumFaces;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			unsigned int indice = face.mIndices[j] + loaded_vertex;//as we merge all mesh, we have to take in account the already loaded ones

			if (this->_dataMode == BP_LINEAR) {
				vertices.push_back(this->assimpProcessVertexAt(mesh, scene, indice));
			} else if (this->_dataMode == BP_INDICES) {
				this->_indices.push_back(indice);
			}

		}
	}
}

void	Obj3dBP::assimpProcessNode(aiNode* node, const aiScene* scene, std::vector<SimpleVertex>& vertices) {
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->assimpProcessMesh(mesh, scene, vertices);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		this->assimpProcessNode(node->mChildren[i], scene, vertices);
	}
}

void	Obj3dBP::loadWithAssimp(string path) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	//https://stackoverflow.com/questions/39269121/assimp-loader-with-a-cube-of-8-vertices
	Assimp::Importer importer;
	//http://assimp.sourceforge.net/lib_html/postprocess_8h.html
	unsigned int flags = aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_PreTransformVertices | aiProcess_OptimizeGraph;
	const aiScene* scene = importer.ReadFile(path, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	//string directory = path.substr(0, path.find_last_of("/\\"));
	vector<SimpleVertex> vertices;
	this->assimpProcessNode(scene->mRootNode, scene, vertices);
	this->normalize(vertices);
	this->initBuffers(vertices);
	std::cout << __PRETTY_FUNCTION__ << " END" << std::endl;
}

Obj3dBP::Obj3dBP(string filename) : Blueprint(filename) {
	cout << "_ Obj3dBP cons by filename" << endl;
	filename = Misc::crossPlatPath(filename);

	this->_dataMode = Obj3dBP::defaultDataMode;
	this->_polygonAmount = 0;
	this->_centered = Obj3dBP::center;
	this->_rescaled = Obj3dBP::rescale;
	this->loadWithAssimp(filename);
	//this->loadWithTinyobj(filename);

	std::cout << this->_indices.size() << std::endl;
	std::cout << this->_indices.size() << std::endl;

	std::cout << __PRETTY_FUNCTION__ << " END" << std::endl;
	std::cout << "----------------------------------------\n" << std::endl;
}

Obj3dBP::Obj3dBP(const Obj3dBP& src) : Blueprint(src) {
	cout << "_ Obj3dBP cons by copy" << endl;
	cout << "building object: " << src.getName().c_str() << endl;

	*this = src;
}

Obj3dBP::~Obj3dBP() {
	cout << "_ Obj3dBP des by filename" << endl;
	/*
		delete opengl data here
	*/
}

Obj3dBP& Obj3dBP::operator=(const Obj3dBP& src) {
	cout << "_ Obj3dBP operator =" << endl;
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
uint8_t			Obj3dBP::getDataMode(void) const { return this->_dataMode; }
const GLuint*	Obj3dBP::getIndicesData(void) const { return this->_indices.data(); }
GLuint			Obj3dBP::getVboVertex(void) const { return this->_vboVertex; }
GLuint			Obj3dBP::getVboColor(void) const { return this->_vboColor; }
GLuint			Obj3dBP::getVboTexture(void) const { return this->_vboTexture; }
int				Obj3dBP::getPolygonAmount(void) const { return this->_polygonAmount; }
Math::Vector3	Obj3dBP::getDimensions(void) const { return this->_dimensions; }
bool			Obj3dBP::isCentered(void) const { return this->_centered; }
bool			Obj3dBP::isRescaled(void) const { return this->_rescaled; }

void	Obj3dBP::normalize(std::vector<SimpleVertex>& vertices) {
	// dimensions and centering
	float	vmin[3];
	float	vmax[3];
	vmin[0] = vertices[0].position.x;
	vmin[1] = vertices[1].position.x;
	vmin[2] = vertices[2].position.x;
	vmax[0] = vmin[0];
	vmax[1] = vmin[1];
	vmax[2] = vmin[2];
	for (size_t i = 0; i < vertices.size(); i++) {
		tinyobj::real_t vx = vertices[i].position.x;
		tinyobj::real_t vy = vertices[i].position.y;
		tinyobj::real_t vz = vertices[i].position.z;
		vmin[0] = (vx < vmin[0]) ? vx : vmin[0];
		vmin[1] = (vy < vmin[1]) ? vy : vmin[1];
		vmin[2] = (vz < vmin[2]) ? vz : vmin[2];
		vmax[0] = (vx > vmax[0]) ? vx : vmax[0];
		vmax[1] = (vy > vmax[1]) ? vy : vmax[1];
		vmax[2] = (vz > vmax[2]) ? vz : vmax[2];

	}
	this->_dimensions.x = vmax[0] - vmin[0];
	this->_dimensions.y = vmax[1] - vmin[1];
	this->_dimensions.z = vmax[2] - vmin[2];

	Math::Vector3	centerOffset;
	centerOffset.x = (vmin[0] + vmax[0]) / 2;
	centerOffset.y = (vmin[1] + vmax[1]) / 2;
	centerOffset.z = (vmin[2] + vmax[2]) / 2;
	if (this->_centered) {
		for (size_t i = 0; i < vertices.size(); i++) {
			vertices[i].position.x -= centerOffset.x;
			vertices[i].position.y -= centerOffset.y;
			vertices[i].position.z -= centerOffset.z;
		}
	}

	float	scaleCoef = 1.0f;
	if (this->_rescaled) {
		scaleCoef = calcScaleCoef(this->_dimensions, Obj3dBP::defaultSize);
		this->_dimensions.mult(scaleCoef);
		for (size_t i = 0; i < vertices.size(); i++) {
			vertices[i].position.mult(scaleCoef);
		}
	}

	std::cout << "polygons { " << this->_polygonAmount << " }" << endl;
	std::cout << "vertices array size { " << vertices.size() << " }" << endl;
	std::cout << "indices array size { " << this->_indices.size() << " }" << endl;
	std::cout << "center offset:\t" << centerOffset.x << " " << centerOffset.y << " " << centerOffset.z << endl;
	std::cout << "scale coef:\t" << scaleCoef << endl;

	if (LOGFILES) {
		std::stringstream logs;
		int c1 = 0;
		logs << endl;
		logs << "total polygons:\t" << c1 << endl;
		logs << "polygons { " << this->_polygonAmount << " }" << endl;
		logs << "rescaled: " << (this->_rescaled ? "yes" : "no") << endl;
		logs << "scale coef:\t" << scaleCoef << endl;
		logs << "centered: " << (this->_centered ? "yes" : "no") << endl;
		logs << "center offset:\t" << centerOffset.x << " " << centerOffset.y << " " << centerOffset.z << endl;

		//data
		logs << "vertices:\t";
		for (size_t i = 0; i < vertices.size(); i++) {
			logs << vertices[i].position.x << " ";
			logs << vertices[i].position.y << " ";
			logs << vertices[i].position.z << ", ";
		}
		logs << endl << "size: " << vertices.size() << " : " << vertices.size() << endl;
		Misc::logfile(std::string("Obj3dBP_vao_") + std::to_string(this->_vao), logs.str());
	}

}

void	Obj3dBP::initBuffers(std::vector<SimpleVertex>& vertices) {
	//loading blueprint with opengl
	//VAO
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	//https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
	// glDrawElements
	if (this->_dataMode == BP_INDICES) {
		glGenBuffers(1, &this->_eboIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_eboIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indices.size() * sizeof(float), this->_indices.data(), GL_STATIC_DRAW);
	}
	//VBO
	glGenBuffers(1, &this->_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SimpleVertex), vertices.data(), GL_STATIC_DRAW);//1 vbo for everything

	//end
	glBindBuffer(GL_ARRAY_BUFFER, 0);//attribution will be done later with a program
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