#pragma once

#include "unittests.hpp"
#include "math.hpp"
#include <cmath>
#define ULP	4

#define TEST_MATH_UTIL		true
#define TEST_MATH_VECTOR3	true
#define TEST_MATH_ROTATION	true
#define TEST_MATH_MATRIX4	true

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
}				t_var;

class MathTests {
public:
	t_var	v;//private?

	void	setUp();
	void	tearDown();
	void	testEverything();

	void	test_ToRadian();
	void	test_ToDegree();
	// void	testExtractFromMatrix();

	// Vector3
	void	testVector3();
	void	testVector3_operatorEqual();
	void	testVector3_rotate();
	void	testVector3_translate();
	void	testVector3_add();
	void	testVector3_sub();
	void	testVector3_mult();
	void	testVector3_div();
	void	testVector3_magnitude();
	void	testVector3_operatorMinus(); // in fact its for the sign minus
	void	testVector3_cross();
	void	testVector3_dot();
	
	// Rotation
	void	testRotation();
	void	testRotation_OperatorEqual();
	void	testRotation_SetAsRad();
	void	testRotation_SetAsDegree();
	void	testRotation_SetUnit(uint8_t unit);
	void	testRotation_ToRadian();
	void	testRotation_ToDegree();
	void	testRotation_IsRadian();
	void	testRotation_IsDegree();

/*
	// Matrix4
	void	testMatrix4();
*/
};