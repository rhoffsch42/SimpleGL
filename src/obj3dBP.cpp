#include "simplegl.h"
#include "obj3dBP.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

float			Obj3dBP::defaultSize = OBJ3DBP_DEFAULT_SIZE;
uint8_t			Obj3dBP::dataMode = BP_INDICES;
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

Obj3dBP::Obj3dBP(string filename) : Blueprint(filename) {
	cout << "_ Obj3dBP cons by filename" << endl;

	filename = Misc::crossPlatPath(filename);
	std::string		cwd(Misc::getCurrentDirectory());
	size_t	i = filename.find_last_of("/\\");
	if (i != std::string::npos) {
		change_cwd(filename.substr(0, i).c_str());//change to the obj directory to be able to load other files like .mtl
		filename = filename.substr(i + 1);
	}

	//loading 
	tinyobj::attrib_t					attrib;
	std::vector<tinyobj::shape_t>		shapes;
	std::vector<tinyobj::material_t>	materials;
	std::string		err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());
	if (!err.empty())
		cerr << err << endl;
	if (!ret) {
		cerr << "tinyobj: failed to load " << filename << endl;
		Misc::breakExit(1);
	}
	if (i != std::string::npos)
		change_cwd(cwd.c_str());

	std::stringstream logs;
	int c1 = 0;
	if (LOGFILES) {
		logs << std::fixed << std::setprecision(2);

		logs << filename << endl;
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
	tinyobj::attrib_t	attribUsed;
	float	vmin[3];
	float	vmax[3];
	vmin[0] = attrib.vertices[0];
	vmin[1] = attrib.vertices[1];
	vmin[2] = attrib.vertices[2];
	vmax[0] = vmin[0];
	vmax[1] = vmin[1];
	vmax[2] = vmin[2];

	this->dataMode = Obj3dBP::dataMode;

	if (this->dataMode == BP_VERTEX_ARRAY) {
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {

			if (LOGFILES) {
				logs << "group:\t" << shapes[s].mesh.num_face_vertices.size() << " polygons" << endl;
				c1 += shapes[s].mesh.num_face_vertices.size();
			}
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];
				if (fv != 3) {
					cerr << filename << " : bad obj format, polygons must be triangles only" << endl;
					Misc::breakExit(2);
				}
				// Loop over vertices in the face.
				for (int v = 0; v < fv; v++) {
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					//	printf("v: %.2f, %.2f, %.2f\n", vx, vy, vz);
					attribUsed.vertices.push_back(vx);
					attribUsed.vertices.push_back(vy);
					attribUsed.vertices.push_back(vz);

					//for dimensions and centering
					vmin[0] = (vx < vmin[0]) ? vx : vmin[0];
					vmin[1] = (vy < vmin[1]) ? vy : vmin[1];
					vmin[2] = (vz < vmin[2]) ? vz : vmin[2];
					vmax[0] = (vx > vmax[0]) ? vx : vmax[0];
					vmax[1] = (vy > vmax[1]) ? vy : vmax[1];
					vmax[2] = (vz > vmax[2]) ? vz : vmax[2];


					if (attrib.normals.size()) {
						tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
						tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
						tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
						//	printf("n: %.2f, %.2f, %.2f\n", nx, ny, nz);
						attribUsed.normals.push_back(nx);
						attribUsed.normals.push_back(ny);
						attribUsed.normals.push_back(nz);
					}
					if (attrib.colors.size()) {
						int x = shapes[s].mesh.material_ids[f];
						tinyobj::real_t r = materials[x].diffuse[0];
						tinyobj::real_t g = materials[x].diffuse[1];
						tinyobj::real_t b = materials[x].diffuse[2];
						//	printf("c: %.2f, %.2f, %.2f\n", r, g, b);
						attribUsed.colors.push_back(r);
						attribUsed.colors.push_back(g);
						attribUsed.colors.push_back(b);
					}
					if (attrib.texcoords.size()) {
						tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
						tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
						//	printf("t: %.2f, %.2f\n", tx, ty);
						attribUsed.texcoords.push_back(tx);
						attribUsed.texcoords.push_back(ty);
					}
				}
				//	cout << endl;
				index_offset += fv;
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	else if (this->dataMode == BP_INDICES) {
		/*
			typedef struct {
			  std::vector<real_t> vertices;   // 'v'
			  std::vector<real_t> normals;    // 'vn'
			  std::vector<real_t> texcoords;  // 'vt'
			  std::vector<real_t> colors;     // extension: vertex colors
			} attrib_t;
		*/
		attribUsed = attrib;//copying

		for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
			tinyobj::real_t vx = attrib.vertices[i + 0];
			tinyobj::real_t vy = attrib.vertices[i + 1];
			tinyobj::real_t vz = attrib.vertices[i + 2];
			//for dimensions and centering
			vmin[0] = (vx < vmin[0]) ? vx : vmin[0];
			vmin[1] = (vy < vmin[1]) ? vy : vmin[1];
			vmin[2] = (vz < vmin[2]) ? vz : vmin[2];
			vmax[0] = (vx > vmax[0]) ? vx : vmax[0];
			vmax[1] = (vy > vmax[1]) ? vy : vmax[1];
			vmax[2] = (vz > vmax[2]) ? vz : vmax[2];
		}

		// Loop over shapes
		this->elem_count = 0;
		for (size_t s = 0; s < shapes.size(); s++) {
			this->elem_count += shapes[s].mesh.indices.size();
			for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {
				this->_indices.push_back(shapes[s].mesh.indices[i].vertex_index);
			}
		}
		//normals, textures and colors not done
	}
	else {
		std::cout << __PRETTY_FUNCTION__ << " : data corrupt:\n\twrong dataMode: " << (int)this->dataMode << std::endl;
		Misc::breakExit(23);
	}

	this->_faceAmount = (int)attribUsed.vertices.size() / 9;
	this->_dimensions.x = vmax[0] - vmin[0];
	this->_dimensions.y = vmax[1] - vmin[1];
	this->_dimensions.z = vmax[2] - vmin[2];

	Math::Vector3	centerOffset;
	centerOffset.x = (vmin[0] + vmax[0]) / 2;
	centerOffset.y = (vmin[1] + vmax[1]) / 2;
	centerOffset.z = (vmin[2] + vmax[2]) / 2;
	if ((this->_centered = Obj3dBP::center)) {
		for (size_t i = 0; i < attribUsed.vertices.size(); i += 3) {
			attribUsed.vertices[i + 0] -= centerOffset.x;
			attribUsed.vertices[i + 1] -= centerOffset.y;
			attribUsed.vertices[i + 2] -= centerOffset.z;
		}
	}

	float	scaleCoef = 1.0f;
	if ((this->_rescaled = Obj3dBP::rescale)) {
		scaleCoef = calcScaleCoef(this->_dimensions, Obj3dBP::defaultSize);
		this->_dimensions.mult(scaleCoef);
		for (size_t i = 0; i < attribUsed.vertices.size(); i++) {
			attribUsed.vertices[i] *= scaleCoef;
		}
	}

	std::cout << "faces { " << this->_faceAmount << " }" << endl;
	std::cout << "color { " << (int)attribUsed.colors.size() / 9 << " }" << endl;
	std::cout << "center offset:\t" << centerOffset.x << " " << centerOffset.y << " " << centerOffset.z << endl;
	std::cout << "scale coef:\t" << scaleCoef << endl;

	if (LOGFILES) {
		logs << endl;
		logs << "total polygons:\t" << c1 << endl;
		logs << "faces { " << this->_faceAmount << " }" << endl;
		logs << "color { " << (int)attribUsed.colors.size() / 9 << " }" << endl;
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

		Misc::logfile(string("Obj3dBP_" + filename), logs.str());
	}

	//loading blueprint with opengl
	//VAO
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glEnableVertexAttribArray(0);
	//VBO vertex
	glGenBuffers(1, &this->_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, attribUsed.vertices.size() * sizeof(float), attribUsed.vertices.data(), GL_STATIC_DRAW);
	//VBO color
	if (attribUsed.colors.size()) {
		glGenBuffers(1, &this->_vboColor);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vboColor);
		glBufferData(GL_ARRAY_BUFFER, attribUsed.colors.size() * sizeof(float), attribUsed.colors.data(), GL_STATIC_DRAW);
	}
	//VBO texture
	if (attribUsed.texcoords.size()) {
		glGenBuffers(1, &this->_vboTexture);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vboTexture);
		glBufferData(GL_ARRAY_BUFFER, attribUsed.texcoords.size() * sizeof(float), attribUsed.texcoords.data(), GL_STATIC_DRAW);
	}
	else if (true) {//VBO texture manual (cube)
		cout << "manualvbotexture" << endl;
		glGenBuffers(1, &this->_vboTexture);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vboTexture);
		for (int i = 0; i < this->_faceAmount; i++) {
			if (i % 2 == 0) {
				attribUsed.texcoords.push_back(1);
				attribUsed.texcoords.push_back(1);
				attribUsed.texcoords.push_back(0);
				attribUsed.texcoords.push_back(1);
				attribUsed.texcoords.push_back(0);
				attribUsed.texcoords.push_back(0);
			}
			else {
				attribUsed.texcoords.push_back(1);
				attribUsed.texcoords.push_back(1);
				attribUsed.texcoords.push_back(0);
				attribUsed.texcoords.push_back(0);
				attribUsed.texcoords.push_back(1);
				attribUsed.texcoords.push_back(0);
			}
		}
		glBufferData(GL_ARRAY_BUFFER, attribUsed.texcoords.size() * sizeof(float), attribUsed.texcoords.data(), GL_STATIC_DRAW);
	}
	//VBO normals (todo)
	// ...

	//end
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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

Obj3dBP &		Obj3dBP::operator=(const Obj3dBP& src) {
	cout << "_ Obj3dBP operator =" << endl;
	//what do we do for vbo? see .hpp
	this->_dataMode = src._dataMode;
	this->_indices = src._indices;
	this->_faceAmount = src._faceAmount;
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
int				Obj3dBP::getFaceAmount(void) const { return this->_faceAmount; }
Math::Vector3	Obj3dBP::getDimensions(void) const { return this->_dimensions; }
bool			Obj3dBP::isCentered(void) const { return this->_centered; }
bool			Obj3dBP::isRescaled(void) const { return this->_rescaled; }

