#pragma once
#include "math.hpp"

class Properties {
#if UNIT_TESTS == true
	friend class UnitTests;
#endif
	friend class Object;
	friend class Obj3d;
	friend class Cam;
public:
	Properties();
	Properties(const Properties& src);
	~Properties();
	Properties&	operator=(const Properties& src);

	float	forwardDistance(Math::Vector3 target_pos);

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
	bool			updateMatrix();
	//relative mutators (default: Math::Vector3::add)
	void			translate(float x, float y, float z);
	void			translate(Math::Vector3 pos);
	void			rotate(float x, float y, float z);// in degree!
	void			rotate(Math::Rotation rot);
	
	// rotate as if rotatePoint was the origin of the object
	void			rotateAround(Math::Vector3 rotatePoint, Math::Rotation rot, float rotWay = ROT_WAY);

	void			enlarge(float x, float y, float z);
	void			enlarge(Math::Vector3 scale);
	//mutators
	void			setPos(float x, float y, float z);
	void			setPos(Math::Vector3 pos);
	void			setRot(float x, float y, float z);// in degree!
	void			setRot(Math::Rotation rot);
	void			setScale(float x, float y, float z);
	void			setScale(Math::Vector3 scale);
	//accessors
	Math::Matrix4&	getMatrix(void) const;//dangerous
	Math::Vector3	getPos(void) const;
	Math::Rotation	getRot(void) const;
	Math::Vector3	getScale(void) const;

	bool			_matrixChanged;
private://public ?
	Math::Matrix4	_matrix;
	bool			_matrixUpdated;

	Math::Vector3	_pos;
	Math::Rotation	_rot;
	Math::Vector3	_scale;
};
