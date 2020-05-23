#pragma once

#include "obj3d.hpp"
#include "program.hpp"

#include <string>


/*
	instance rendering with the BP of the first Obj3d in the list
*/
class Obj3dIPG : public Obj3dPG
{
public:
	Obj3dIPG(std::string vertexShader, std::string fragmentShader, bool init_locations = true);
	~Obj3dIPG();

	virtual void	render(Object& object, Math::Matrix4 VPmatrix) const;//HAS to be cpy constructor! (1 PV*M per obj3d)
	virtual void	renderObjects(list<Object*>& list, Cam& cam, uint8_t flags = 0);
protected:
	virtual void	getLocations();
private:
	GLuint			_vboMatrix;
	int				_vboSize;//amount of matrices stored
	//is the vbo linked to 1 vao, what if we render 2 lists of 2 different obj3d?
};
