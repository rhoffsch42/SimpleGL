#pragma once

#include "program.hpp"
#include "obj3d.hpp"
class Obj3d;
#include "math.hpp"
#include "misc.hpp"

//X faces * 2 polygons * 3 indices * size of indices in opengl
#define EBO_OFFSET(X)	((X) * 2 * 3 * sizeof(GLuint))


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"//?
/*
	https://stackoverflow.com/questions/18515183/c-overloaded-virtual-function-warning-by-clang
	different amount of parameters for render()
*/
class Obj3dPG : public Program
{
public:
	Obj3dPG(std::string vs_file, std::string fs_file, bool init_locations = true);
	~Obj3dPG();
	void	linkBuffers(const Obj3dBP& blueprint) const;
	virtual void	render(Object& object, Math::Matrix4 VPmatrix) const; //HAS to be cpy constructor! (1 PV*M per obj3d)
	void	renderUniqueId(Obj3d & obj, Math::Matrix4 VPmatrix) const;//HAS to be cpy constructor! (1 PV*M per obj3d)
	virtual void	renderObjects(list<Object*>& list, Cam& cam, unsigned int flags = 0);

	/*
		https://cpp.developpez.com/faq/cpp/?page=Les-fonctions-membres-virtuelles
		render could overwrite :
			virtual void Program::render(Obj & obj, Math:Matrix4 mat);// set as virtual pure ?
			where Obj & obj = obj3d_car; //(Obj3d obj3d_car)
	*/
protected:
	
	//slots
	GLint		_mat4_mvp;
	GLint		_dismod;
	GLint		_plain_color;
	GLint		_tex_coef;
	GLint		_vertex_position_data;
	GLint		_vertex_color_data;
	GLint		_vertex_UV_data;
	//slots end
	virtual void	getLocations();
private:

	friend void	scene_test();
};
#pragma clang diagnostic pop
