#pragma once
#include "program.hpp"
#include "math.hpp"
#include "skybox.hpp"
class Skybox;


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
/*
	https://stackoverflow.com/questions/18515183/c-overloaded-virtual-function-warning-by-clang
	different amount of parameters for render()
*/
class SkyboxPG : public Program
{
public:
	SkyboxPG(std::string vs_file, std::string fs_file);
	~SkyboxPG();

	virtual void	renderObjects(list<Object*>& list, Cam& cam, unsigned int flags = 0);
	virtual void	render(Object& object, Math::Matrix4 VPmatrix) const;
	/*
		https://cpp.developpez.com/faq/cpp/?page=Les-fonctions-membres-virtuelles
		render could overwrite :
			virtual void Program::render(...);
				does nothing, Program should not be instanciated
				because we should have render(...) = 0;
				
			/!\ pb: can't change params of virtual pure
			-> render(Object& o, Math::Matrix4& VPmatrix) = 0;
				can be: Object& o = skybox; //(Skybox skybox)
					need class Skybox : public Object {...}
				can be: Object& o = obj; //(Obj3d obj)
			
			/!\ what happen if we render Object with the wrong program:
				Obj3dPG prog_obj3d(...);
				Skybox	skybox(...);
				prog_obj3d.render(skybox, PVMatrix);//very bad
			-> Program::render(...) = 0; as protected
				to have: derived::render(); as private;
				and friend class Obj3d; in Obj3dPG
					to be able to use this->program.render(...);
				and friend class Skybox; in SkyboxPG
					to be able to use this->program.render(...);
		
		But we could just remove Program::render(...);
		and set it in SkyboxPG and Obj3dPG; (like now)
			SkyboxPG::render(Skybox sky, ..PV);
			Obj3dPG::render(Obj3dPG obj, ..PV);
	*/
	//accessors
	GLint	getSlotCubemap() const;

	virtual void	getLocations();
private:

	//slots
	GLint	_mat4_vp;
	GLint	_vertex_position_data;
	GLint	_cubemap;

};
#pragma clang diagnostic pop
