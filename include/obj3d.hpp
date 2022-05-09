#pragma once
#include "texture.hpp"
#include "object.hpp"
#include "obj3dBP.hpp"
#include "obj3dPG.hpp"
class Obj3dPG;
#include "math.hpp"
#include "cam.hpp"

#define OBJ3D_DEFAULT_SIZE	4.0f
#define OBJ3D_DEFAULT_COLOR	Math::Vector3(255.0f, 0.0f, 255.0f);//purple

class Obj3d : public Object
{
public:
	static unsigned int		getInstanceAmount();//overwrite from Object
	static bool				defaultDisplayTexture;
	static GLenum			defaultPolygonMode;
	static Math::Vector3	defaultColor;

	Obj3d(Obj3dBP& bp, Obj3dPG& pg);
	Obj3d(const Obj3d& obj3d);
	~Obj3d();
	Obj3d&	operator=(const Obj3d& obj3d);

	void			render(Math::Matrix4& PVmatrix);

	void			setColor(uint8_t x, uint8_t y, uint8_t z);// 0 -> 255
	void			setTexture(Texture* texture);
	//GL_POINT, GL_LINE, and GL_FILL
	void			setPolygonMode(GLenum mode);
	// void			setMotionBehavior(/*...*/);//useless if _motionBehavior is public

	//accessors
	Obj3dBP*		getBlueprint(void) const;
	Obj3dPG*		getProgram(void) const;
	Math::Vector3	getColor(void) const;
	Math::Vector3	getColorShader(void) const;
	Texture*		getTexture(void) const;
	GLenum			getPolygonMode(void) const;

	//settings
	bool			displayTexture;
protected:
private:
	//overwrite from Object
	static unsigned int	_instanceAmount;	// should never be altered manually

	Obj3dBP&		_blueprint;
	Obj3dPG&		_program;
	Math::Vector3	_color;
	Math::Vector3	_colorShader;
	Texture*		_texture;
	GLenum			_polygonMode;
};
