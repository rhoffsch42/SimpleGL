#pragma once
#include "texture.hpp"
#include "obj3dBP.hpp"
#include "obj3dPG.hpp"
class Obj3dPG;
#include "math.hpp"

class Obj3d
{
public:
	static int	instanceAmount;//private? should never be altered manually
	static int	instanceId;//private? should never be altered manually
	static bool		defaultCentered;
	static bool		defaultRotate;
	static bool		defaultDisplayTexture;
	static GLenum	defaultPolygonMode;

//	Obj3d(); obj vide ? oui mais plutot Obj avec seulement une modelmatrix, donc une autre classe ?
/*	-> faire heriter Obj3d : Obj ?
		class Obj {
			[...]
			rotate
			id
			modelmatrix
			matrixUpdated
			pos
			rot
			scale
		}
*/	
	Obj3d(Obj3dBP& bp, Obj3dPG& pg);
	Obj3d(const Obj3d& obj3d);
	~Obj3d();
	Obj3d&	operator=(const Obj3d& obj3d);

	void	render(Math::Matrix4& PVmatrix);
	/*
		set a movement attribute function
		ex: rotate on Y, and move along a range on PosY (like a floating bonus in games_
		void	setMvtFunc(void *(func)(Obj3d& obj)) {
			this->_mvtFunc = func;
		}
		then in each frame, call this->_mvtFunc()
		ie: func(...) {
			this->pos.y += (cos(time) * DELTA);
			this->rot.y += DEG * DELTA; // care rad/degree
		}
	*/

	//mutators
	void			setPos(float x, float y, float z);
	void			setPos(Math::Vector3 pos);
	void			setRot(float x, float y, float z);// in degree!
	void			setRot(Math::Rotation rot);
	void			setScale(float x, float y, float z);
	void			setScale(Math::Vector3 scale);
	void			setColor(uint8_t x, uint8_t y, uint8_t z);
	void			setTexture(Texture* texture);
	void			setPolygonMode(GLenum mode);

	//accessors
	unsigned int	getId() const;
	Obj3dBP&		getBlueprint() const;
	Obj3dPG&		getProgram() const;
	Math::Matrix4&	getModelMatrix() const;
	Math::Vector3	getPos() const;
	Math::Rotation	getRot() const;
	Math::Vector3	getScale() const;
	Math::Vector3	getColor() const;
	Texture*		getTexture() const;
	GLenum			getPolygonMode() const;

	//setings
	bool			_displayTexture;
	bool			_rotate;
	bool			_centered;
protected:
private:
	unsigned int	_id;
	Obj3dBP&		_blueprint;
	Obj3dPG&		_program;
	Math::Matrix4	_modelMatrix;
	bool			_matrixUpdated;
	Math::Vector3	_pos;
	Math::Rotation	_rot;
	Math::Vector3	_scale;
	Math::Vector3	_color;
	Texture*		_texture;
	GLenum			_polygonMode;

	void	center();

};
