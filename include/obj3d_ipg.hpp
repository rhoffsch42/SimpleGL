#pragma once

#include "obj3dPG.hpp"

#include <string>

class Obj3dIPG : public Obj3dPG
{
public:
	Obj3dIPG(std::string vertexShader, std::string fragmentShader);
	~Obj3dIPG();

	void	render(Object& object, Math::Matrix4 VPmatrix) const;//HAS to be cpy constructor! (1 PV*M per obj3d)
	void	renderObjects(list<Object*>& list, Cam& cam, bool force_draw = false);
	void	getLocations();
private:
};
