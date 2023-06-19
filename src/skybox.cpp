#include "simplegl.h"
#include "skybox.hpp"

#ifdef SGL_DEBUG
 #define SGL_SKYBOX_DEBUG
#endif
#ifdef SGL_SKYBOX_DEBUG 
 #define D(x) std::cout << "[Skybox] " << x ;
 #define D_(x) x
 #define D_SPACER "-- skybox.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

Skybox::Skybox(Texture& texture, SkyboxPG& pg) : Object(),  _program(pg), _baseTexture(texture) {
	D(__PRETTY_FUNCTION__ << "\n")

	//Cubemap texture
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &this->_cubemapId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->_cubemapId);
	
	unsigned int	baseHeight = this->_baseTexture.getHeight();
	unsigned int	baseWidth = this->_baseTexture.getWidth();
	unsigned int	height = baseHeight / 3;
	unsigned int	width = baseWidth / 4;

	unsigned int	index[6][2] = {
		{ 1,2 }, //right
		{ 1,0 }, //left
		{ 0,1 }, //top
		{ 2,1 }, //bottom
		{ 1,1 }, //front
		{ 1,3 } //back
	};

	uint8_t*	p = this->_baseTexture.getData();
	uint8_t*	data = new uint8_t[height*width*3];//*3 cauz RGB
	unsigned int startH, endH, startW, endW;
	unsigned int i;
	//building all faces
	for (int face = 0; face < 6; face++) {
		startH = height * (index[face][0] + 0);
		endH = height * (index[face][0] + 1);
		startW = width * (index[face][1] + 0);
		endW = width * (index[face][1] + 1);

		i = 0;
		for (unsigned int h = startH; h < endH; h++) {
			for (unsigned int w = startW; w < endW; w++) {
				// flipped because cubemap sucks ass (follows Top Left convention)
				unsigned int ind = ((baseHeight - h - 1) * baseWidth + w) * 3;
				/* not flipped version (you have to invert top and bottom in index[6][2] too!
				unsigned int ind = (h * baseWidth + w) * 3;
				*/
				data[i + 0] = p[ind + 0];
				data[i + 1] = p[ind + 1];
				data[i + 2] = p[ind + 2];
				i += 3;
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	delete[] data;
	glUniform1i(this->_program.getSlotCubemap(), 0);//a quoi ca sert ?

	float	l = SKYBOX_LEN / 2.0f;
	//6(faces) * 2(triangles) * 3(vertex) * 3(xyz) = 108 floats
	float	vertex[108] = {
		l, -l, -l,		l, -l, l,		l, l, l,//right
		l, l, l,		l, l, -l,		l, -l, -l,

		-l, -l, l,		-l, -l, -l,		-l, l, -l,//left
		-l, l, -l,		-l, l, l,		-l, -l, l,

		-l, l, -l,		l, l, -l,		l, l, l,//top
		l, l, l,		-l, l, l,		-l, l, -l,

		-l, -l, -l,		-l, -l, l,		l, -l, -l,//bottom
		l, -l, -l,		-l, -l, l,		l, -l, l,

		-l, l, -l,		-l, -l, -l,		l, -l, -l,//front
		l, -l, -l,		l, l, -l,		-l, l, -l,

		-l, -l, l,		-l, l, l,		l, l, l,//back
		l, l, l,		l, -l, l,		-l, -l, l
	};

	//VAO vertex
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glEnableVertexAttribArray(0);
	//VBO vertex
	glGenBuffers(1, &this->_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), vertex, GL_STATIC_DRAW);
}

Skybox::~Skybox() {
	D(__PRETTY_FUNCTION__ << "\n")
	glDeleteTextures(1, &this->_cubemapId);
	glDeleteBuffers(1, &this->_vboVertex);
	glDeleteVertexArrays(1, &this->_vao);
}

void			Skybox::render(Math::Matrix4& VPmatrix) const {
	this->_program.render((Skybox&)(*this), VPmatrix);
}

//accessors
SkyboxPG&		Skybox::getProgram() const { return ((SkyboxPG&)this->_program); }
GLuint			Skybox::getCubemapId() const { return (this->_cubemapId); }
GLuint			Skybox::getVao() const { return (this->_vao); }
GLuint			Skybox::getVboVertex() const { return (this->_vboVertex); }
