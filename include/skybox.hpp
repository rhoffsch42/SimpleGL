#pragma once
#include "texture.hpp"
#include "skyboxPG.hpp"
class SkyboxPG;
#include "math.hpp"

#define SKYBOX_LEN	10.0f

class Skybox
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