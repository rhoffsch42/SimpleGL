#pragma once
#include "object.hpp"
#include "texture.hpp"
#include "skyboxPG.hpp"
class SkyboxPG;
#include "math.hpp"

#define SKYBOX_LEN	400000.0f

class Skybox : public Object
{
public:
	Skybox(Texture& texture, SkyboxPG& pg);
	~Skybox();

	void	render(Math::Matrix4& VPmatrix) const;

	//accessors
	SkyboxPG&		getProgram() const;
	GLuint			getCubemapId() const;
	GLuint			getVao() const;
	GLuint			getVboVertex() const;
private:
	SkyboxPG&	_program;
	Texture&	_baseTexture;
	GLuint		_cubemapId;
	GLuint		_vao;
	GLuint		_vboVertex;
};
