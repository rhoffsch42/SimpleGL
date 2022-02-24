#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <iomanip>

#define EMOTE_WARNING		"\xe2\x9a\xa0\xef\xb8\x8f " // ⚠️

//Math::Rotation : Vector3
//Math::Matrix4
#define COS_X	val[0]
#define SIN_X	val[1]
#define COS_Y	val[2]
#define SIN_Y	val[3]
#define COS_Z	val[4]
#define SIN_Z	val[5]

//Math::Rotation : Vector3
#define ROT_DEG	0
#define ROT_RAD	1

//define this in Math::Vector3::
#define VEC3_LEFT		Math::Vector3(-1, 0, 0)
#define VEC3_RIGHT		Math::Vector3(1, 0, 0)
#define VEC3_DOWN		Math::Vector3(0, -1, 0)
#define VEC3_UP			Math::Vector3(0, 1, 0)
#define VEC3_BACKWARD	Math::Vector3(0, 0, 1)
#define VEC3_FORWARD	Math::Vector3(0, 0, -1)

//Math::Matrix4
#define ROW_MAJOR		0
#define COLUMN_MAJOR	1
#define ROT_RIGHT		-1
#define ROT_LEFT		1
#define ROT_WAY			1


/*
	Math
	Math::Vector3
	Math::Rotation : Vector3
	Math::Matrix4
*/

typedef struct s_pp	t_pp;

class Math
{
public:
	Math();
	~Math();

	class Matrix4;
	class Rotation;
	
	//static
	static float		toRadian(float degree);
	static float		toDegree(float radian);
	static t_pp			extractFromMatrix(Math::Matrix4 matrix);

	class Vector3 {
	public:
		float	x;
		float	y;
		float	z;

		Vector3();
		Vector3(float valx, float valy, float valz);
		Vector3(const Math::Vector3& src);

		void	rotate(Math::Rotation rot, float rotWay);
		void	rotate(float x, float y, float z, float rotWay);//degree
		void	ZYXrotate(Math::Rotation rot, float rotWay);
		
		// rotate as if rotatePoint was the origin of the world
		void	rotateAround(Math::Vector3 rotatePoint, Math::Rotation rot, float rotWay = ROT_WAY);
		/*
			? Is it possible to avoid doing that : 
				void	rotateAround(Math::Vector3 rotatePoint, float rotX, float rotY, float rotZ, float rotWay = ROT_WAY);
				void	rotateAround(float posX, float posY, float posZ, Math::Rotation rot, float rotWay = ROT_WAY);
				void	rotateAround(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotWay = ROT_WAY);
		*/

		//vector operation
		void	add(Math::Vector3 v);
		void	add(float valx, float valy, float valz);
		void	sub(Math::Vector3 v);
		void	sub(float valx, float valy, float valz);
		void	mult(float coef);
		void	mult(Math::Matrix4& mat);
		void	div(float coef);
		void	scale(Math::Vector3 v);//Multiplies the vectors component-wise. 
		float	len2() const;
		float	len() const;
		void	normalize();
		Math::Vector3	normalized() const;

		//operators
		Math::Vector3	operator-() const; //the sign minus, not the mathematical operator for substractions
		void			operator+=(const Math::Vector3& rhs);
		void			operator-=(const Math::Vector3& rhs);
		void			operator*=(const float& rhs);
		void			operator/=(const float& rhs);
		Math::Vector3	operator+(const Math::Vector3& rhs) const;
		Math::Vector3	operator-(const Math::Vector3& rhs) const;
		Math::Vector3	operator*(const float& rhs) const;
		Math::Vector3	operator/(const float& rhs) const;
		friend Math::Vector3 operator*(float lhs, const Math::Vector3& rhs) { return rhs * lhs; }
		friend Math::Vector3 operator/(float lhs, const Math::Vector3& rhs) { return rhs / lhs; }
		friend std::ostream& operator<<(std::ostream& os, const Math::Vector3& vec) { return os << vec.toString(); }
		bool			operator<(const Math::Vector3& rhs) const;//to be used as functor
		bool			operator==(const Math::Vector3& rhs) const;
		bool			operator!=(const Math::Vector3& rhs) const;

		//static
		// https://physics.info/vector-multiplication/
		static Math::Vector3	cross(Math::Vector3 v1, Math::Vector3 v2);// ie mult
		static float			dot(Math::Vector3 v1, Math::Vector3 v2);// = scalar product
		//other
		std::string		toString() const;
	};

	class Rotation : public Vector3
	{
#if UNIT_TESTS == true
		friend class UnitTests;
#endif
	public:
		Rotation();//default unit is degree
		Rotation(float valx, float valy, float valz);//default unit is degree
		Rotation(float valx, float valy, float valz, uint8_t unit);
		Rotation(const Rotation& src);
		Rotation&	operator=(const Rotation& src);
		~Rotation();

		void		setAsRad();
		void		setAsDegree();
		void		setUnit(uint8_t unit);
		Rotation	toRadian() const;
		Rotation	toDegree() const;
		bool		isRadian() const;
		bool		isDegree() const;
	private:
		uint8_t	_unit;//radian or degree
	};

	class Matrix4
	{
#if UNIT_TESTS == true
		friend class UnitTests;
#endif
	public:
		Matrix4();
		Matrix4(const Matrix4& src);
		Matrix4&	operator=(const Matrix4& mat);
		~Matrix4();

		//add scale
		void		orthogonalMatrix(float left, float right, float bottom, float top);
		void		projectionMatrix(float fovRad, float farv, float nearv, int win_x, int win_y);
		void		viewMatrix(Math::Vector3 camPos, Math::Rotation camRot);
		void		modelMatrix(Math::Vector3 pos, Math::Rotation rot, Math::Vector3 scale);
		void		scaleMatrix(Math::Vector3 scale);
		void		identity();
		void		reset();
		void		setOrder(uint8_t order);//not a mutator
		void		transpose();
		void		printData() const;

		void		updatePosValue(Math::Vector3 pos);
		//matrix operation
		void		div(float coef);
		void		mult(float coef);
		void		mult(Math::Matrix4& mat);

		//static
		//mutator
		//accessor
		float*			getData() const;
		uint8_t			getOrder() const;

	private:
		/*
			Manipulation with tab 4x4
			Important when COLUMN_MAJOR order:
				the indexes are not the same as mathematical order, they are inverted
				ie: tab[0][1] is in fact the first number of the second row!
		*/
		union {
			float	e[16];
			float	tab[4][4];
		};
		uint8_t		_order;
	};
};

//inline functions https://stackoverflow.com/questions/953710/inline-function-linker-error
inline void		Math::Matrix4::identity() {
	e[0] = e[5] = e[10] = e[15] = 1.0f;
	e[1] = e[2] = e[3] = e[4] = e[6] = e[7] = e[8] = e[9] = e[11] = e[12] = e[13] = e[14] = 0.0f;
}
inline void		Math::Matrix4::reset() {
	e[0] = e[5] = e[10] = e[15] = 0.0f;
	e[1] = e[2] = e[3] = e[4] = e[6] = e[7] = e[8] = e[9] = e[11] = e[12] = e[13] = e[14] = 0.0f;
}

typedef struct	s_pp {
	Math::Vector3	pos;
	Math::Rotation	rot;
	Math::Vector3	scale;
}				t_pp;
