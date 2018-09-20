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

	void	render(Skybox& skybox, Math::Matrix4& VPmatrix);//tmp args, still ?
	/*
		https://cpp.developpez.com/faq/cpp/?page=Les-fonctions-membres-virtuelles
		render could overwrite :
			virtual void Program::render(Obj & obj, Math:Matrix4 mat);
			where Obj & obj = skybox; //(Skybox skybox)
	*/
	//accessors
	GLint	getSlotCubemap() const;
private:
	void	getLocations();

	//slots
	GLint	_mat4_vp;
	GLint	_vertex_position_data;
	GLint	_cubemap;

};

