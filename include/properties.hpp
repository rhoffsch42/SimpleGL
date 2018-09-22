#pragma once
#include "math.hpp"

#define PP_DEFAULT_SIZE	4.0f

class Properties
{
public:
	static bool				defaultCentered;
	static float			defaultSize;
	static bool				defaultRescaled;

	Properties();
	Properties(float scaleCoef);
	Properties(const Properties& src);
	~Properties();
	Properties&	operator=(const Properties& src);

	/*
		to change the operation on relative mutators (and avoid if-else forest),
		set this in Math::Vector3:
			static float		(*operation_ptr)(float,float);
		can be:	std::plus<float>()		(default)
				std::minus<float>()
				std::multiplies<float>()
			void	Math::Vector3::operation(float valx, float valy, float valz) {
				this->x = Math::Vector3::operation_ptr(this->x, valx);
				this->y = Math::Vector3::operation_ptr(this->y, valy);
				this->z = Math::Vector3::operation_ptr(this->z, valz);
			}
			void	Math::Vector3::operation(Math::Vector3 vec) {
				this->x = Math::Vector3::operation_ptr(this->x, vec.x);
				this->y = Math::Vector3::operation_ptr(this->y, vec.y);
				this->z = Math::Vector3::operation_ptr(this->z, vec.z);
			}

		can have a func to change operation_ptr here
		(dont think it is proper to the instance of Properties):
			void	setOperation(float (*operation_ptr)(float,float)) {//not sure about syntax
				Math::Vector3::operation_ptr = &std::plus<float>();
			}	
		to then use:
			Properties::setOperation(&std::XFUNC<float>());
			Math::Vector3::operation_ptr = &std::XFUNC<float>();
		then in relative mutators:
			this->_pos.operation(Math::Vector3);
			this->_pos.operation(x, y, z);
	*/
	//relative mutators (default: Math::Vector3::add)
	void			translate(float x, float y, float z);
	void			translate(Math::Vector3 pos);
	void			rotate(float x, float y, float z);// in degree!
	void			rotate(Math::Rotation rot);
	void			enlarge(float x, float y, float z);
	void			enlarge(Math::Vector3 scale);
	//mutators
	void			setPos(float x, float y, float z);
	void			setPos(Math::Vector3 pos);
	void			setRot(float x, float y, float z);// in degree!
	void			setRot(Math::Rotation rot);
	void			setScale(float x, float y, float z);
	void			setScale(Math::Vector3 scale);
	void			setRescaled(bool value);
	//accessors
	Math::Matrix4&	getMatrix(void) const;
	Math::Vector3	getPos(void) const;
	Math::Rotation	getRot(void) const;
	Math::Vector3	getScale(void) const;
	Math::Vector3	getFinalScale(void) const;
	float			getScaleCoef(void) const;
	bool			isRescaled(void) const;
	Math::Vector3	getCenterOffset(void) const;

	bool			centered;
private:
	Math::Matrix4	_matrix;
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
	Math::Vector3	_centerOffset;
 	void	center();
};

// class Object
// {
// public:
// 	static unsigned int		getInstanceAmount();
// 	static bool				defaultCentered;

// 	Object();
// 	Object(const Object& obj);
// 	~Object();
// 	Object&	operator=(const Object& obj);

// 	void			runMothionBehavior(void * ptr);
// 	virtual void	render(Math::Matrix4& PVmatrix) = 0;

// 	//mutators
// 	void			setPos(float x, float y, float z);
// 	void			setPos(Math::Vector3 pos);
// 	void			setRot(float x, float y, float z);// in degree!
// 	void			setRot(Math::Rotation rot);
// 	void			setScale(float x, float y, float z);
// 	void			setScale(Math::Vector3 scale);
// 	void			setRescaled(bool value);
// 	void			setColor(uint8_t x, uint8_t y, uint8_t z);
// 	void			setTexture(Texture* texture);
// 	void			setPolygonMode(GLenum mode);
// 	// void			setMotionBehavior(/*...*/);//useless if _motionBehavior is public

// 	//accessors
// 	unsigned int	getId(void) const;
// 	Obj3dBP&		getBlueprint(void) const;
// 	Obj3dPG&		getProgram(void) const;
// 	Math::Matrix4&	getModelMatrix(void) const;
// 	Math::Vector3	getPos(void) const;
// 	Math::Rotation	getRot(void) const;
// 	Math::Vector3	getScale(void) const;
// 	Math::Vector3	getFinalScale(void) const;
// 	float			getScaleCoef(void) const;
// 	bool			isRescaled(void) const;
// 	Math::Vector3	getColor(void) const;
// 	Texture*		getTexture(void) const;
// 	GLenum			getPolygonMode(void) const;
// 	void			(*getMotionBehaviorFunc(void) const) (Obj3d &, void*);

// 	//settings
// 	bool			centered;
// 	bool			_motionBehavior;
// 	//motion behavior
// 	void			(*_motionBehaviorFunc)(Obj3d & ref, void* ptr);
// 	/*
// 		The static variables behavior must NOT depend on the object properties.
// 		As it does not belong to the Obj3d instance,
// 		its static variables will be altered by all instances using it.
// 	*/
// protected:
// 	static unsigned int	_instanceAmount;	// should never be altered manually
// 	static unsigned int	_instanceId;		// should never be altered manually
	
// 	unsigned int	_id;
// 	Math::Matrix4 &	_localToWorld_Matrix;// = parent's worldMatrix
// 	/*
// 		reference ou pointer? si je mets une reference const,
// 		la matrice peut elle etre modifiee via sa classe "originelle"
// 	*/

// 	Properties		_world;
// 	Properties		_local;

// 	Math::Matrix4	_worldMatrix;//= _localToWorld_Matrix * _localMatrix
// 	Math::Matrix4	_localMatrix;
// 	bool			_worldMatrixUpdated;
// 	bool			_localMatrixUpdated;

// 	Math::Vector3	_worldPos;
// 	Math::Vector3	_localPos;
// 	Math::Rotation	_worldRot;
// 	Math::Rotation	_localRot;
// 	Math::Vector3	_worldScale;
// 	Math::Vector3	_localScale;

// 	void	center();
// private:

// };
