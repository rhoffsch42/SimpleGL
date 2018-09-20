#pragma once
#include "texture.hpp"
#include "obj3dBP.hpp"
#include "obj3dPG.hpp"
class Obj3dPG;
#include "math.hpp"

#define OBJ3D_DEFAULT_SIZE	4.0f//duplicate in Obj

class Obj3d
{
public:
	static unsigned int		getInstanceAmount();//duplicate this in Obj
	static bool				defaultCentered;//Obj
	static bool				defaultDisplayTexture;
	static GLenum			defaultPolygonMode;
	static float			defaultSize;//Obj
	static bool				defaultRescaled;//Obj

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

	void	runMothionBehavior(void * ptr);//Obj
	void	render(Math::Matrix4& PVmatrix);//Obj? virtual pure ?

	//mutators
	void			setPos(float x, float y, float z);//Obj
	void			setPos(Math::Vector3 pos);//Obj
	void			setRot(float x, float y, float z);// in degree!//Obj
	void			setRot(Math::Rotation rot);//Obj
	void			setScale(float x, float y, float z);//Obj
	void			setScale(Math::Vector3 scale);//Obj
	void			setRescaled(bool value);//Obj
	void			setColor(uint8_t x, uint8_t y, uint8_t z);
	void			setTexture(Texture* texture);
	void			setPolygonMode(GLenum mode);
	// void			setMotionBehavior(/*...*/);//useless if _motionBehavior is public

	//accessors
	unsigned int	getId(void) const;//Obj
	Obj3dBP&		getBlueprint(void) const;
	Obj3dPG&		getProgram(void) const;
	Math::Matrix4&	getModelMatrix(void) const;//Obj
	Math::Vector3	getPos(void) const;//Obj
	Math::Rotation	getRot(void) const;//Obj
	Math::Vector3	getScale(void) const;//Obj
	Math::Vector3	getFinalScale(void) const;//Obj
	float			getScaleCoef(void) const;//Obj
	bool			isRescaled(void) const;//Obj
	Math::Vector3	getColor(void) const;
	Texture*		getTexture(void) const;
	GLenum			getPolygonMode(void) const;
	void			(*getMotionBehaviorFunc(void) const) (Obj3d &, void*);//Obj

	//settings
	bool			displayTexture;
	bool			centered;//Obj
	bool			_motionBehavior;//Obj
	//motion behavior
	void			(*_motionBehaviorFunc)(Obj3d & ref, void* ptr);//Obj
	/*
		The static variables behavior must NOT depend on the object properties.
		As it does not belong to the Obj3d instance,
		its static variables will be altered by all instances using it.
	*/
protected:
private:
	//duplicate this in Obj (amount, Obj3d amount will be included in Obj amount)
	static unsigned int	_instanceAmount;	// should never be altered manually
	static unsigned int	_instanceId;//obj		// should never be altered manually

	unsigned int	_id;//Obj
	Obj3dBP&		_blueprint;
	Obj3dPG&		_program;
	Math::Matrix4	_modelMatrix;//Obj
	bool			_matrixUpdated;//Obj
	Math::Vector3	_pos;//Obj
	Math::Rotation	_rot;//Obj
	Math::Vector3	_scale;//Obj
	Math::Vector3	_finalScale;//Obj
	const float		_scaleCoef;//Obj
	bool			_rescaled;//Obj
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
