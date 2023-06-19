#include "simplegl.h"
#include "math.hpp"
#include <iostream>
#include <sstream>
#include <tuple>

//#ifdef SGL_DEBUG
 #define SGL_MATH_DEBUG
//#endif
#ifdef SGL_MATH_DEBUG 
 #define D(x) std::cout << "[Math] " << x ;
 #define D_(x) x
 #define D_SPACER "-- math.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

Math::Math() {}
Math::~Math() {}

//static
float	Math::toRadian(float degree) {
	return (degree * float(M_PI) / 180.0f);
}
float	Math::toDegree(float radian) {
	return (radian * (180.0f / float(M_PI)));
}

t_pp	Math::extractFromMatrix(Math::Matrix4 mat) {
	mat.setOrder(ROW_MAJOR);
	s_pp	pp;
	float	(&m)[4][4] = *reinterpret_cast<float(*)[4][4]>(mat.getData());

	pp.pos.x = m[0][3];
	pp.pos.y = m[1][3];
	pp.pos.z = m[2][3];

	Math::Vector3	vectorX;
	Math::Vector3	vectorY;
	Math::Vector3	vectorZ;
	vectorX.x = m[0][0];
	vectorX.y = m[1][0];
	vectorX.z = m[2][0];
	vectorY.x = m[0][1];
	vectorY.y = m[1][1];
	vectorY.z = m[2][1];
	vectorZ.x = m[0][2];
	vectorZ.y = m[1][2];
	vectorZ.z = m[2][2];

	pp.scale.x = vectorX.len();
	pp.scale.y = vectorY.len();
	pp.scale.z = vectorZ.len();
	m[0][0] /= pp.scale.x;
	m[1][0] /= pp.scale.x;
	m[2][0] /= pp.scale.x;
	m[0][1] /= pp.scale.y;
	m[1][1] /= pp.scale.y;
	m[2][1] /= pp.scale.y;
	m[0][2] /= pp.scale.z;
	m[1][2] /= pp.scale.z;
	m[2][2] /= pp.scale.z;

	if (m[0][2] < 1) {
		if (m[0][2] > -1) {
			pp.rot.y = asinf(m[0][2]);
			pp.rot.x = atan2(-m[1][2], m[2][2]);
			pp.rot.z = atan2(-m[0][1], m[0][0]);
		} else {	//m[0][2] = -1 (sin(y) = -1 (+-180 deg))
			pp.rot.y = -M_PI / 2.0f;
			pp.rot.x = -atan2(m[1][0], m[1][1]);
			pp.rot.z = 0;
		}
	} else {	//m[0][2] = +1 (sin(y) = +1 (+-0 deg))
		pp.rot.y = M_PI / 2.0f;
		pp.rot.x = atan2(m[1][0], m[1][1]);
		pp.rot.z = 0;
	}
	pp.rot.setUnit(ROT_RAD);
	return (pp);
}

/////////////////////////////////////////////////////////////////////////////
//	class Vector3
Math::Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
Math::Vector3::Vector3(float valx, float valy, float valz) : x(valx), y(valy), z(valz) {}
Math::Vector3::Vector3(const Math::Vector3& src) {
	*this = src;
}
//Math::Vector3&	Math::Vector3::operator=(const Math::Vector3& src) {
//	this->x = src.x;
//	this->y = src.y;
//	this->z = src.z;
//	return (*this);
//}
//	Vector3 member func
void			Math::Vector3::rotate(Math::Rotation rot, float rotWay) {
	float		val[8];
	float		mat[16];
	Vector3		tmp;

	rot.setAsRad();//important
	rot.x *= rotWay;
	rot.y *= rotWay;
	rot.z *= rotWay;
	COS_X = cosf(rot.x);
	SIN_X = sinf(rot.x);
	COS_Y = cosf(rot.y);
	SIN_Y = sinf(rot.y);
	COS_Z = cosf(rot.z);
	SIN_Z = sinf(rot.z);
	val[6] = COS_X * SIN_Y;
	val[7] = SIN_X * SIN_Y;

	mat[0] = COS_Y * COS_Z;
	mat[1] = -COS_Y * SIN_Z;
	mat[2] = SIN_Y;
	mat[4] = val[7] * COS_Z + COS_X * SIN_Z;
	mat[5] = -val[7] * SIN_Z + COS_X * COS_Z;
	mat[6] = -SIN_X * COS_Y;
	mat[8] = -val[6] * COS_Z + SIN_X * SIN_Z;
	mat[9] = val[6] * SIN_Z + SIN_X * COS_Z;
	mat[10] = COS_X * COS_Y;
	tmp.x = this->x * mat[0] + this->y * mat[1] + this->z * mat[2];
	tmp.y = this->x * mat[4] + this->y * mat[5] + this->z * mat[6];
	tmp.z = this->x * mat[8] + this->y * mat[9] + this->z * mat[10];
	this->x = tmp.x;
	this->y = tmp.y;
	this->z = tmp.z;
}
void			Math::Vector3::rotate(float x, float y, float z, float rotWay) {
	this->rotate(Math::Rotation(x, y, z, ROT_DEG), rotWay);
}
void			Math::Vector3::ZYXrotate(Math::Rotation rot, float rotWay) {
	float		val[8];
	float		mat[16];
	Vector3		tmp;

	rot.setAsRad();//important
	rot.x *= rotWay;
	rot.y *= rotWay;
	rot.z *= rotWay;
	COS_X = cosf(rot.x);
	SIN_X = sinf(rot.x);
	COS_Y = cosf(rot.y);
	SIN_Y = sinf(rot.y);
	COS_Z = cosf(rot.z);
	SIN_Z = sinf(rot.z);
	val[6] = COS_X * SIN_Y;
	val[7] = SIN_X * SIN_Y;

	mat[0] = COS_Y * COS_Z;
	mat[1] = -SIN_Z * COS_X + val[7] * COS_Z;
	mat[2] = SIN_Z * SIN_X + val[6] * COS_Z;
	mat[4] = COS_Y * SIN_Z;
	mat[5] = COS_X * COS_Z + val[7] * SIN_Z;
	mat[6] = -COS_Z * SIN_X + val[6] * SIN_Z;
	mat[8] = -SIN_Y;
	mat[9] = SIN_X * COS_Y;
	mat[10] = COS_X * COS_Y;

	tmp.x = this->x * mat[0] + this->y * mat[1] + this->z * mat[2];
	tmp.y = this->x * mat[4] + this->y * mat[5] + this->z * mat[6];
	tmp.z = this->x * mat[8] + this->y * mat[9] + this->z * mat[10];
	this->x = tmp.x;
	this->y = tmp.y;
	this->z = tmp.z;
}

/*
	Math::Vector3::rotateAround is based from this implementation:
	https://gamedev.stackexchange.com/questions/59843/rotating-an-object-when-the-center-in-not-the-origin-opengl
	Easy way of building the rotation matrix :
		1	Start with an identity matrix
		2	Translate the matrix by - centre of the object
		3	Rotate the matrix by the desired amount
		4	Translate the matrix by centre of the object
		5	Use the resulting matrix to transform the object that you desire to rotate
*/
void			Math::Vector3::rotateAround(Math::Vector3 rotatePoint, Math::Rotation rot, float rotWay) {
		this->sub(rotatePoint);
		this->rotate(rot, rotWay);
		this->add(rotatePoint);
}

//vector operation
void			Math::Vector3::add(Math::Vector3 v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}
void			Math::Vector3::add(float valx, float valy, float valz) {
	this->x += valx;
	this->y += valy;
	this->z += valz;
}
void			Math::Vector3::sub(Math::Vector3 v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}
void			Math::Vector3::sub(float valx, float valy, float valz) {
	this->x -= valx;
	this->y -= valy;
	this->z -= valz;
}
void			Math::Vector3::mult(float coef) {
	this->x *= coef;
	this->y *= coef;
	this->z *= coef;
}
void			Math::Vector3::mult(Math::Matrix4& mat) {
	uint8_t	order = mat.getOrder();
	mat.setOrder(ROW_MAJOR);
	float* m = mat.getData();
	Math::Vector3	res;
	res.x = this->x * m[0] + this->y * m[1] + this->z * m[2] + 1 * m[3];
	res.y = this->x * m[4] + this->y * m[5] + this->z * m[6] + 1 * m[7];
	res.z = this->x * m[8] + this->y * m[9] + this->z * m[10] + 1 * m[11];
	*this = res;
	mat.setOrder(order);
}
void			Math::Vector3::div(float coef) {
	this->x /= coef;
	this->y /= coef;
	this->z /= coef;
}
void			Math::Vector3::scale(Math::Vector3 v) {
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
}
float			Math::Vector3::len2() const {
	return (x * x + y * y + z * z);
}
float			Math::Vector3::len() const {
	return (sqrt(x * x + y * y + z * z));
}
void			Math::Vector3::normalize() {
	float len = this->len();
	if (len > 0) {
		this->div(len);
	}
}
Math::Vector3	Math::Vector3::normalized() const {
	Math::Vector3 vec = *this;
	vec.normalize();
	return (vec);
}

//operators
//Math::Vector3::operator*<int>(int n) const;
Math::Vector3	Math::Vector3::operator-(void) const {
	return (Math::Vector3(-this->x, -this->y, -this->z));
}
void			Math::Vector3::operator+=(const Math::Vector3& rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
}
void			Math::Vector3::operator-=(const Math::Vector3& rhs) {
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
}
void			Math::Vector3::operator*=(const float& rhs) {
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;
}
void			Math::Vector3::operator/=(const float& rhs) {
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
}
Math::Vector3	Math::Vector3::operator+(const Math::Vector3& rhs) const {
	Math::Vector3 vec(*this);
	vec += rhs;
	return vec;
}
Math::Vector3	Math::Vector3::operator-(const Math::Vector3& rhs) const {
	Math::Vector3 vec(*this);
	vec -= rhs;
	return vec;
}
Math::Vector3	Math::Vector3::operator*(const float& rhs) const {
	Math::Vector3 vec(*this);
	vec *= rhs;
	return vec;
}
Math::Vector3	Math::Vector3::operator/(const float& rhs) const {
	Math::Vector3 vec(*this);
	vec /= rhs;
	return vec;
}
bool			Math::Vector3::operator<(const Math::Vector3& rhs) const {
	return std::tie(x, y, z) < std::tie(rhs.x, rhs.y, rhs.z);
}
bool			Math::Vector3::operator==(const Math::Vector3& rhs) const {
	return (x == rhs.x && y == rhs.y && z == rhs.z);
}
bool			Math::Vector3::operator!=(const Math::Vector3& rhs) const {
	return !(*this == rhs);
}
//static
Math::Vector3	Math::Vector3::cross(Math::Vector3 v1, Math::Vector3 v2) {
	return (Math::Vector3(	v1.y * v2.z - v1.z * v2.y, \
							v1.z * v2.x - v1.x * v2.z, \
							v1.x * v2.y - v1.y * v2.x) );
}
float			Math::Vector3::dot(Math::Vector3 v1, Math::Vector3 v2) {
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}
std::string		Math::Vector3::toString() const {
	std::stringstream ss;
	ss << "(" << this->x << ", " << this->y << ", " << this->z << ")";
	return ss.str();
}
//	Vector3 end
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//	class Rotation : public Vector3
Math::Rotation::Rotation() : Vector3() {
	this->_unit = ROT_DEG;
}
Math::Rotation::Rotation(float valx, float valy, float valz) : Vector3(valx, valy, valz) {
	this->_unit = ROT_DEG;
}
Math::Rotation::Rotation(float valx, float valy, float valz, uint8_t unit) : Vector3(valx, valy, valz), _unit(unit) {}
Math::Rotation::Rotation(const Rotation& src) {
	*this = src;
}
Math::Rotation&	Math::Rotation::operator=(const Rotation& src) {
	Vector3::operator=(src);//check si cest bon
	if (src.isDegree())
		this->_unit = ROT_DEG;
	else
		this->_unit = ROT_RAD;
	return (*this);
}
Math::Rotation::~Rotation() {}
void			Math::Rotation::setAsRad() {
	if (this->_unit == ROT_DEG) {
		this->x = Math::toRadian(this->x);
		this->y = Math::toRadian(this->y);
		this->z = Math::toRadian(this->z);
		this->_unit = ROT_RAD;
	}
}
void			Math::Rotation::setAsDegree() {
	if (this->_unit == ROT_RAD) {
		this->x = Math::toDegree(this->x);
		this->y = Math::toDegree(this->y);
		this->z = Math::toDegree(this->z);
		this->_unit = ROT_DEG;
	}
}
void			Math::Rotation::setUnit(uint8_t unit) {
	this->_unit = unit;
}
Math::Rotation	Math::Rotation::toRadian() const {
	Rotation	vec(this->x, this->y, this->z, this->_unit);
	vec.setAsRad();
	return (vec);
}
Math::Rotation	Math::Rotation::toDegree() const {
	Rotation	vec(this->x, this->y, this->z, this->_unit);
	vec.setAsDegree();
	return (vec);
}
bool			Math::Rotation::isRadian() const {
	return (this->_unit == ROT_RAD);
}
bool			Math::Rotation::isDegree() const {
	return (this->_unit == ROT_DEG);
}
//	Rotation end
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//	class Matrix4
Math::Matrix4::Matrix4() {
	this->identity();
	this->_order = ROW_MAJOR;
}
Math::Matrix4::Matrix4(const Matrix4& src) {
	*this = src;
}
Math::Matrix4&	Math::Matrix4::operator=(const Math::Matrix4& src) {
	float*	s = src.getData();
	for (int i = 0; i < 16; i++)
		this->e[i] = s[i];
	this->_order = src.getOrder();
	return (*this);
}
Math::Matrix4::~Matrix4() {}
void	Math::Matrix4::orthogonalMatrix(float left, float right, float bottom, float top) {
	this->identity();
	this->_order = ROW_MAJOR;
	this->tab[0][0] = 2 / (right - left);
	this->tab[1][1] = 2 / (top - bottom);
	this->tab[2][2] = -1;
	this->tab[3][0] = -(right + left) / (right - left);
	this->tab[3][1] = -(top + bottom) / (top - bottom);
	this->setOrder(COLUMN_MAJOR);
}
void			Math::Matrix4::projectionMatrix(float fovRad, float farv, float nearv, int win_x, int win_y) {
	this->reset();
	this->_order = ROW_MAJOR;
	float	tanFovHalf = tanf(fovRad / 2.0f);
	float	aspectRatio = (float)win_x / (float)win_y;
	float	zRange = nearv - farv;
	this->tab[0][0] = 1.0f / (tanFovHalf * aspectRatio);
	this->tab[1][1] = 1.0f / tanFovHalf;
	this->tab[2][2] = -(nearv + farv) / zRange;
	this->tab[2][3] = -2.0f * farv * nearv / zRange;
	this->tab[3][2] = -1.0f;
	this->setOrder(COLUMN_MAJOR);
}
void			Math::Matrix4::viewMatrix(Math::Vector3 camPos, Math::Rotation camRot) {
	//D(__PRETTY_FUNCTION__ << "\taddr: " << this << std::endl)
	float		val[8];
	
	this->identity();
	this->_order = ROW_MAJOR;
	camRot.setAsRad();//important
	COS_X = cosf(camRot.x);
	SIN_X = sinf(camRot.x);
	COS_Y = cosf(camRot.y);
	SIN_Y = sinf(camRot.y);
	COS_Z = cosf(camRot.z);
	SIN_Z = sinf(camRot.z);
	val[6] = COS_X * SIN_Y;
	val[7] = SIN_X * SIN_Y;
	this->tab[0][0] = COS_Y * COS_Z;
	this->tab[1][0] = val[7] * COS_Z + COS_X * SIN_Z;
	this->tab[2][0] = -val[6] * COS_Z + SIN_X * SIN_Z;
	this->tab[0][1] = -COS_Y * SIN_Z;
	this->tab[1][1] = -val[7] * SIN_Z + COS_X * COS_Z;
	this->tab[2][1] = val[6] * SIN_Z + SIN_X * COS_Z;
	this->tab[0][2] = SIN_Y;
	this->tab[1][2] = -SIN_X * COS_Y;
	this->tab[2][2] = COS_X * COS_Y;
	camPos.rotate(camRot, ROT_WAY);
	this->tab[0][3] = -camPos.x;
	this->tab[1][3] = -camPos.y;
	this->tab[2][3] = -camPos.z;
	this->setOrder(COLUMN_MAJOR);
}
void			Math::Matrix4::modelMatrix(Math::Vector3 pos, Math::Rotation rot, Math::Vector3 scale) {//add scale
	float		val[8];

	this->reset();
	rot.setAsRad();//important
	COS_X = cosf(rot.x);
	SIN_X = sinf(rot.x);
	COS_Y = cosf(rot.y);
	SIN_Y = sinf(rot.y);
	COS_Z = cosf(rot.z);
	SIN_Z = sinf(rot.z);
	val[6] = COS_X * SIN_Y;
	val[7] = SIN_X * SIN_Y;
	this->tab[0][0] = COS_Y * COS_Z;
	this->tab[1][0] = val[7] * COS_Z + COS_X * SIN_Z;
	this->tab[2][0] = -val[6] * COS_Z + SIN_X * SIN_Z;
	this->tab[0][1] = -COS_Y * SIN_Z;
	this->tab[1][1] = -val[7] * SIN_Z + COS_X * COS_Z;
	this->tab[2][1] = val[6] * SIN_Z + SIN_X * COS_Z;
	this->tab[0][2] = SIN_Y;
	this->tab[1][2] = -SIN_X * COS_Y;
	this->tab[2][2] = COS_X * COS_Y;
	this->tab[0][3] = pos.x;
	this->tab[1][3] = pos.y;
	this->tab[2][3] = pos.z;
	this->tab[3][3] = 1.0f;
	this->_order = ROW_MAJOR;//because above calculations are ROW_MAJOR
	
	//optimised this->mult(scaleMatrix);
	this->tab[0][0] *= scale.x;
	this->tab[1][0] *= scale.x;
	this->tab[2][0] *= scale.x;
	this->tab[0][1] *= scale.y;
	this->tab[1][1] *= scale.y;
	this->tab[2][1] *= scale.y;
	this->tab[0][2] *= scale.z;
	this->tab[1][2] *= scale.z;
	this->tab[2][2] *= scale.z;
/*	equivalent to:
	Math::Matrix4	scaleMatrix;
	scaleMatrix.scaleMatrix(scale);
	this->mult(scaleMatrix);
*/
	this->setOrder(COLUMN_MAJOR);
}
void			Math::Matrix4::scaleMatrix(Math::Vector3 scale) {
	// no reset ? or identity ? what if we do: m.modelMatrix(); m.scale(1, 1, 1);
	this->tab[0][0] = scale.x;
	this->tab[1][1] = scale.y;
	this->tab[2][2] = scale.z;
}
/*
 moved in the header
inline void		Math::Matrix4::identity() {
	e[0] = e[5] = e[10] = e[15] = 1.0f;
	e[1] = e[2] = e[3] = e[4] = e[6] = e[7] = e[8] = e[9] = e[11] = e[12] = e[13] = e[14] = 0.0f;
}
inline void		Math::Matrix4::reset() {
	e[0] = e[5] = e[10] = e[15] = 0.0f;
	e[1] = e[2] = e[3] = e[4] = e[6] = e[7] = e[8] = e[9] = e[11] = e[12] = e[13] = e[14] = 0.0f;
}
*/
void			Math::Matrix4::setOrder(uint8_t order) {
	if (order == this->_order)
		return;
	this->_order = order;
	this->transpose();
}
void			Math::Matrix4::transpose() {
	Math::Matrix4	cp(*this);
	float			*v = cp.getData();

	e[1] = v[4];	e[2] = v[8];	e[3] = v[12];
	e[4] = v[1];	e[6] = v[9];	e[7] = v[13];
	e[8] = v[2];	e[9] = v[6];	e[11] = v[14];
	e[12] = v[3];	e[13] = v[7];	e[14] = v[11];
}
void			Math::Matrix4::printData() const {
	std::ios oldState(nullptr);
	oldState.copyfmt(std::cout);
	std::cout << std::fixed << std::setprecision(2);

	for (int i = 0; i < 16; i++) {
		std::cout << std::setfill('0') << std::setw(5) << this->e[i] << " ";
		if (i % 4 == 3)
			std::cout << " ";
	}
	std::cout << "\n\n";

	std::cout << "Matrix representation ( " << EMOTE_WARNING << " not the data as a tab[][] )\taddr: " << this << std::endl;
	if (this->_order == ROW_MAJOR)
		std::cout << "row major\n";
	else
		std::cout << "column major\n";

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (this->_order == ROW_MAJOR)
				std::cout << std::setfill('0') << std::setw(5) << this->tab[i][j] << " ";
			else
				std::cout << std::setfill('0') << std::setw(5) << this->tab[j][i] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout.copyfmt(oldState);
}
void			Math::Matrix4::updatePosValue(Math::Vector3 pos) {//inline ?
	if (this->_order == ROW_MAJOR) {
		this->tab[0][3] = pos.x;
		this->tab[1][3] = pos.y;
		this->tab[2][3] = pos.z;
	}
	else if (this->_order == COLUMN_MAJOR) {
		this->tab[3][0] = pos.x;
		this->tab[3][1] = pos.y;
		this->tab[3][2] = pos.z;
	}
	else {
		D("Matrix4::_order impossible case: must be either ROW_MAJOR or COLUMN_MAJOR\n")
		std::exit(EXIT_FAILURE);
	}

}
//matrix operation
void			Math::Matrix4::mult(float coef) {
	for (int i = 0; i < 16; i++)
		this->e[i] *= coef;
}
void			Math::Matrix4::div(float coef) {
	for (int i = 0; i < 16; i++)
		this->e[i] /= coef;
}
void			Math::Matrix4::mult(Math::Matrix4& mat) {
	//save orders
	uint8_t	order1 = this->_order;
	uint8_t	order2 = mat.getOrder();
	//always make calculation with row major order!
	this->setOrder(ROW_MAJOR);
	mat.setOrder(ROW_MAJOR);

	float	res[4][4];
	float	(&m1)[4][4] = this->tab;
	float	(&m2)[4][4] = *reinterpret_cast<float(*)[4][4]>(mat.getData());

	res[0][0] = m1[0][0]*m2[0][0] + m1[0][1]*m2[1][0] + m1[0][2]*m2[2][0] + m1[0][3]*m2[3][0];
	res[0][1] = m1[0][0]*m2[0][1] + m1[0][1]*m2[1][1] + m1[0][2]*m2[2][1] + m1[0][3]*m2[3][1];
	res[0][2] = m1[0][0]*m2[0][2] + m1[0][1]*m2[1][2] + m1[0][2]*m2[2][2] + m1[0][3]*m2[3][2];
	res[0][3] = m1[0][0]*m2[0][3] + m1[0][1]*m2[1][3] + m1[0][2]*m2[2][3] + m1[0][3]*m2[3][3];

	res[1][0] = m1[1][0]*m2[0][0] + m1[1][1]*m2[1][0] + m1[1][2]*m2[2][0] + m1[1][3]*m2[3][0];
	res[1][1] = m1[1][0]*m2[0][1] + m1[1][1]*m2[1][1] + m1[1][2]*m2[2][1] + m1[1][3]*m2[3][1];
	res[1][2] = m1[1][0]*m2[0][2] + m1[1][1]*m2[1][2] + m1[1][2]*m2[2][2] + m1[1][3]*m2[3][2];
	res[1][3] = m1[1][0]*m2[0][3] + m1[1][1]*m2[1][3] + m1[1][2]*m2[2][3] + m1[1][3]*m2[3][3];

	res[2][0] = m1[2][0]*m2[0][0] + m1[2][1]*m2[1][0] + m1[2][2]*m2[2][0] + m1[2][3]*m2[3][0];
	res[2][1] = m1[2][0]*m2[0][1] + m1[2][1]*m2[1][1] + m1[2][2]*m2[2][1] + m1[2][3]*m2[3][1];
	res[2][2] = m1[2][0]*m2[0][2] + m1[2][1]*m2[1][2] + m1[2][2]*m2[2][2] + m1[2][3]*m2[3][2];
	res[2][3] = m1[2][0]*m2[0][3] + m1[2][1]*m2[1][3] + m1[2][2]*m2[2][3] + m1[2][3]*m2[3][3];

	res[3][0] = m1[3][0]*m2[0][0] + m1[3][1]*m2[1][0] + m1[3][2]*m2[2][0] + m1[3][3]*m2[3][0];
	res[3][1] = m1[3][0]*m2[0][1] + m1[3][1]*m2[1][1] + m1[3][2]*m2[2][1] + m1[3][3]*m2[3][1];
	res[3][2] = m1[3][0]*m2[0][2] + m1[3][1]*m2[1][2] + m1[3][2]*m2[2][2] + m1[3][3]*m2[3][2];
	res[3][3] = m1[3][0]*m2[0][3] + m1[3][1]*m2[1][3] + m1[3][2]*m2[2][3] + m1[3][3]*m2[3][3];

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->tab[i][j] = res[i][j];
	//re-establich orders
	this->setOrder(order1);
	mat.setOrder(order2);
}
//mutator
//accessor
float*			Math::Matrix4::getData() const { return (float*)(this->e); }
uint8_t			Math::Matrix4::getOrder() const { return this->_order; }
//	Matrix4 end
/////////////////////////////////////////////////////////////////////////////
