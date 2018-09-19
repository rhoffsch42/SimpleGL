#include "simplegl.h"
#include "obj3dBP.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

Obj3dBP::Obj3dBP(string filename) : Blueprint(filename) {
	cout << "_ Obj3dBP cons by filename" << endl;

	filename = Misc::crossPlatPath(filename);
	std::string		cwd(Misc::getCurrentDirectory());
	size_t	i = filename.find_last_of("/\\");
	if (i != std::string::npos) {
		change_cwd(filename.substr(0, i).c_str());
		filename = filename.substr(i + 1);
	}

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


	std::vector<float>	points;
	std::vector<float>	normals;
	std::vector<float>	colors;
	std::vector<float>	textures;
	float	vmin[3];
	float	vmax[3];
	tinyobj::index_t index = shapes[0].mesh.indices[0];
	tinyobj::real_t vx = attrib.vertices[3 * index.vertex_index + 0];
	vmin[0] = attrib.vertices[3 * index.vertex_index + 0];
	vmin[1] = attrib.vertices[3 * index.vertex_index + 1];
	vmin[2] = attrib.vertices[3 * index.vertex_index + 2];
	vmax[0] = vmin[0];
	vmax[1] = vmin[1];
	vmax[2] = vmin[2];
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {

		// Loop over faces(polygon)
		if (LOGFILES) {
			logs << "group:\t" << shapes[s].mesh.num_face_vertices.size() << " polygons" << endl;
			c1 += shapes[s].mesh.num_face_vertices.size();
		}
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

				vmin[0] = (vx < vmin[0]) ? vx : vmin[0];
				vmin[1] = (vy < vmin[1]) ? vy : vmin[1];
				vmin[2] = (vz < vmin[2]) ? vz : vmin[2];
				vmax[0] = (vx > vmax[0]) ? vx : vmax[0];
				vmax[1] = (vy > vmax[1]) ? vy : vmax[1];
				vmax[2] = (vz > vmax[2]) ? vz : vmax[2];

			//	printf("v: %.2f, %.2f, %.2f\n", vx, vy, vz);
				points.push_back(vx);
				points.push_back(vy);
				points.push_back(vz);
				if (attrib.normals.size()) {
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				//	printf("n: %.2f, %.2f, %.2f\n", nx, ny, nz);
					normals.push_back(nx);
					normals.push_back(ny);
					normals.push_back(nz);
				}
				if (attrib.colors.size()) {
					int x = shapes[s].mesh.material_ids[f];
					tinyobj::real_t r = materials[x].diffuse[0];
					tinyobj::real_t g = materials[x].diffuse[1];
					tinyobj::real_t b = materials[x].diffuse[2];
				//	printf("c: %.2f, %.2f, %.2f\n", r, g, b);
					colors.push_back(r);
					colors.push_back(g);
					colors.push_back(b);
				}
				if (attrib.texcoords.size()) {
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				//	printf("t: %.2f, %.2f\n", tx, ty);
					textures.push_back(tx);
					textures.push_back(ty);
				}
			}
		//	cout << endl;
			index_offset += fv;
			shapes[s].mesh.material_ids[f];
		}
	}
	this->_faceAmount = (int)points.size() / 9;
	this->_centerOffset.x = (vmin[0] + vmax[0]) / 2;
	this->_centerOffset.y = (vmin[1] + vmax[1]) / 2;
	this->_centerOffset.z = (vmin[2] + vmax[2]) / 2;
	this->_dimensions.x = vmax[0] - vmin[0];
	this->_dimensions.y = vmax[1] - vmin[1];
	this->_dimensions.z = vmax[2] - vmin[2];

	std::cout << "faces { " << this->_faceAmount << " }" << endl;
	std::cout << "color { " << (int)colors.size() / 9 << " }" << endl;
	std::cout << "centerOffset:\t" << this->_centerOffset.x << " " << this->_centerOffset.y << " " << this->_centerOffset.z << endl;

	if (LOGFILES) {
		logs << endl;
		logs << "total polygons:\t" << c1 << endl;
		logs << "faces { " << this->_faceAmount << " }" << endl;
		logs << "color { " << (int)colors.size() / 9 << " }" << endl;
		logs << "centerOffset:\t" << this->_centerOffset.x << " " << this->_centerOffset.y << " " << this->_centerOffset.z << endl;

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


	//VAO
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glEnableVertexAttribArray(0);
	//VBO vertex
	glGenBuffers(1, &this->_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);
	//VBO color
	if (colors.size()) {
		glGenBuffers(1, &this->_vboColor);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vboColor);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
	}
	//VBO texture
	if (textures.size()) {
		glGenBuffers(1, &this->_vboTexture);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vboTexture);
		glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(float), textures.data(), GL_STATIC_DRAW);
	}
	else if (true) {//VBO texture manual (cube)
		glGenBuffers(1, &this->_vboTexture);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vboTexture);
		for (int i = 0; i < this->_faceAmount; i++) {
			if (i % 2 == 0) {
			textures.push_back(1);
			textures.push_back(1);
			textures.push_back(0);
			textures.push_back(1);
			textures.push_back(0);
			textures.push_back(0);
			}
			else {
				textures.push_back(1);
				textures.push_back(1);
				textures.push_back(0);
				textures.push_back(0);
				textures.push_back(1);
				textures.push_back(0);
			}
		}
		glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(float), textures.data(), GL_STATIC_DRAW);
	}

}
/*
	float	points[obj->f_amount * summit * 3];
	fill_func(points, obj->f);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, obj->f_amount * summit * 3 * sizeof(float), \
		points, GL_STATIC_DRAW);
*/


Obj3dBP::Obj3dBP(const Obj3dBP& src) : Blueprint(src) {
	cout << "_ Obj3dBP cons by copy" << endl;
	cout << "building object: " << src.getName().c_str() << endl;

	*this = src;
}

Obj3dBP::~Obj3dBP() {
	cout << "_ Obj3dBP des by filename" << endl;
	/*
		should delete opengl data here
	*/
}

Obj3dBP &		Obj3dBP::operator=(const Obj3dBP& src) {
	cout << "_ Obj3dBP operator =" << endl;
	this->_faceAmount = src.getFaceAmount();
	this->_centerOffset = src.getCenterOffset();
	this->_dimensions = src.getDimensions();
	return (*this);
}

//mutators
//accessors
int				Obj3dBP::getFaceAmount(void) const { return (this->_faceAmount); }
Math::Vector3	Obj3dBP::getCenterOffset(void) const { return (this->_centerOffset); }
Math::Vector3	Obj3dBP::getDimensions(void) const { return (this->_dimensions); }
