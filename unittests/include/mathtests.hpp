#pragma once

#include "unittests.hpp"
#include "math.hpp"
#include <cmath>
#define ULP	4

#define TEST_MATH_UTIL		true
#define TEST_MATH_VECTOR3	true
#define TEST_MATH_ROTATION	true
#define TEST_MATH_MATRIX4	true

class MathTests;

template <typename T>
class FreeAccess : public T {
public:
	FreeAccess() : T() {} //if the class T doesnt have default constructor? boom?
	friend class MathTests;
};

typedef struct	s_var {
	float			degree125;
	float			radian2_18166;
	float			coo_1_2_3[3];
	Math::Vector3	vecA_0_0_0;
	Math::Vector3	vecB_4_5_6;
	Math::Vector3	vecC_90_0_m45;
	Math::Vector3	vecD_3_4_5;
	Math::Vector3	vecE_1_1_1;
	Math::Vector3	vecF_m3_6_m3;
	float			magnitude_18;
	float			dot_122;
	Math::Rotation	rotA_0_0_0_d;
	Math::Rotation	rotB_1_2_3_d;
	Math::Rotation	rotC_r;	// 180, 0, 100
	Math::Rotation	rotD_180_0_100_d;
	FreeAccess<Math::Matrix4>	mat4_identity;
	FreeAccess<Math::Matrix4>	mat4_clear;
	FreeAccess<Math::Matrix4>	mat4_0_to_15_RM;
	FreeAccess<Math::Matrix4>	mat4_0_to_15_RM_transposed;
	FreeAccess<Math::Matrix4>	mat4_15_to_0_CM;
	FreeAccess<Math::Matrix4>	mat4_only2_RM;
	FreeAccess<Math::Matrix4>	mat4_only8_RM;
	FreeAccess<Math::Matrix4>	mat4_scale_1_1_1_RM;
	FreeAccess<Math::Matrix4>	mat4_scale_4_5_6_RM;
}				t_var;

class MathTests {
public:
	t_var	v;//private?

	void	setUp();
	void	tearDown();
	void	testEverything();

	void	test_toRadian();
	void	test_toDegree();
	// void	testExtractFromMatrix();

	// Vector3
	void	testVector3_everything();
	void	testVector3();
	void	testVector3_operatorEqual();
	void	testVector3_rotate();
	void	testVector3_translate();
	void	testVector3_add();
	void	testVector3_sub();
	void	testVector3_mult();
	void	testVector3_div();
	void	testVector3_magnitude();
	void	testVector3_operatorMinus(); // in fact its for the sign minus, not the mathematical term operator
	void	testVector3_cross();
	void	testVector3_dot();
	
	// Rotation
	void	testRotation_everything();
	void	testRotation();
	void	testRotation_operatorEqual();
	void	testRotation_setAsRad();
	void	testRotation_setAsDegree();
	void	testRotation_setUnit();
	void	testRotation_toRadian();
	void	testRotation_toDegree();
	void	testRotation_isRadian();
	void	testRotation_isDegree();

	// Matrix4
	void	testMatrix4_everything();
	void	testMatrix4();
	void	testMatrix4_operatorEqual();
	void	testMatrix4_projectionMatrix();
	void	testMatrix4_viewMatrix();
	void	testMatrix4_modelMatrix();
	void	testMatrix4_scaleMatrix();
	void	testMatrix4_identity();
	void	testMatrix4_reset();
	void	testMatrix4_transpose();
	void	testMatrix4_printData();
	void	testMatrix4_updatePosValue();
	void	testMatrix4_div();
	void	testMatrix4_mult();
	void	testMatrix4_getData();
	void	testMatrix4_getOrder();
};