#pragma once
#include "program.hpp"
#include "math.hpp"
#include "skybox.hpp"
class Skybox;

class SkyboxPG : public Program
{
public:
	SkyboxPG(std::string vs_file, std::string fs_file);
	~SkyboxPG();

	void	render(Skybox& skybox, Math::Matrix4& VPmatrix);//tmp args
	//accessors
	GLint	getSlotCubemap() const;
private:
	void	getLocations();

	//slots
	GLint	_mat4_vp;
	GLint	_vertex_position_data;
	GLint	_cubemap;

};

