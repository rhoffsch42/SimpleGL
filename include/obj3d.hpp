#pragma once
#include "texture.hpp"
#include "obj3dBP.hpp"
#include "obj3dPG.hpp"
class Obj3dPG;
#include "math.hpp"

#define OBJ3D_DEFAULT_SIZE	4.0f

class Obj3d
{
public:
	static unsigned int		getInstanceAmount();
	static bool				defaultCentered;
	static bool				defaultRotate;
	static bool				defaultDisplayTexture;
	static GLenum			defaultPolygonMode;
	static float			defaultSize;
	static bool				defaultRescaled;

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

	void	runMothionBehavior(void * ptr);
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
	void			setRescaled(bool value);
	void			setColor(uint8_t x, uint8_t y, uint8_t z);
	void			setTexture(Texture* texture);
	void			setPolygonMode(GLenum mode);
	// void			setMotionBehavior(/*...*/);//useless if _motionBehavior is public

	//accessors
	unsigned int	getId(void) const;
	Obj3dBP&		getBlueprint(void) const;
	Obj3dPG&		getProgram(void) const;
	Math::Matrix4&	getModelMatrix(void) const;
	Math::Vector3	getPos(void) const;
	Math::Rotation	getRot(void) const;
	Math::Vector3	getScale(void) const;
	Math::Vector3	getFinalScale(void) const;
	float			getScaleCoef(void) const;
	bool			isRescaled(void) const;
	Math::Vector3	getColor(void) const;
	Texture*		getTexture(void) const;
	GLenum			getPolygonMode(void) const;
	void			(*getMotionBehaviorFunc(void) const) (Obj3d &, void*);

	//settings
	bool			_displayTexture;
	bool			_rotate;
	bool			_centered;
	bool			_motionBehavior;
	//motion behavior
	void			(*_motionBehaviorFunc)(Obj3d & ref, void* ptr);//private ?
protected:
private:
	static unsigned int	instanceAmount;	// should never be altered manually
	static unsigned int	instanceId;		// should never be altered manually

	unsigned int	_id;
	Obj3dBP&		_blueprint;
	Obj3dPG&		_program;
	Math::Matrix4	_modelMatrix;
	bool			_matrixUpdated;
	Math::Vector3	_pos;
	Math::Rotation	_rot;
	Math::Vector3	_scale;
	Math::Vector3	_finalScale;
	const float		_scaleCoef;
	bool			_rescaled;
	/*
		_scaleCoef should NEVER change once defined!
			If the default size for that specific Obj3d instance has to change,
			the scaleCoef can not be updated subsequently.
			-> Create another Obj3d instead.
		_rescaled has to be private and therefore have a mutator
			because if it changes we need to set _matrixUpdated to false
	*/
	Math::Vector3	_color;
	Texture*		_texture;
	GLenum			_polygonMode;

	void	center();

};
