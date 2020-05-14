#pragma once

#include "obj3d.hpp"
#include "program.hpp"

#include <string>

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
};
