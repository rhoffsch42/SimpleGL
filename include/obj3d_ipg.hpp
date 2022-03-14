#pragma once

#include "obj3d.hpp"
#include "program.hpp"

#include <string>


/*
cube.obj
order with assimp : (probably because it trys to fan / strip tiangles)
  white red green blue purple black
  front right left bottom top back
*/
#define CUBE_FRONT_FACE		0
#define CUBE_RIGHT_FACE		1
#define CUBE_LEFT_FACE		2
#define CUBE_BOTTOM_FACE	3
#define CUBE_TOP_FACE		4
#define CUBE_BACK_FACE		5


/*
	instance rendering with the BP of the first Obj3d in the list
*/
class Obj3dIPG : public Obj3dPG
{
public:
	Obj3dIPG(std::string vertexShader, std::string fragmentShader, bool init_locations = true);
	~Obj3dIPG();

	virtual void	render(Object& object, Math::Matrix4 VPmatrix) const;//HAS to be cpy constructor! (1 PV*M per obj3d)
	virtual void	renderObjects(std::list<Object*>& list, Cam& cam, unsigned int flags = 0);
protected:
	virtual void	getLocations();
private:
	GLuint			_vboMatrix;
	int				_vboSize;//amount of matrices stored
	//is the vbo linked to 1 vao, what if we render 2 lists of 2 different obj3d?
};
