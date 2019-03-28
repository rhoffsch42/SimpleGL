#pragma once

#include "unittests.hpp"
#include "math.hpp"
#include <cmath>
#define ULP	4

#define TEST_MATH_UTIL		true
#define TEST_MATH_VECTOR3	true
#define TEST_MATH_ROTATION	true
#define TEST_MATH_MATRIX4	true

class UnitTests::MathTests {
public:
	static bool	isEqualMat4(const Math::Matrix4 *m1, const Math::Matrix4 *m2);
	static bool	isEqualVector3(const Math::Vector3 *v1, const Math::Vector3 *v2);

	void	setUp();
	void	tearDown();
	void	testEverything() const;

	void	test_toRadian() const;
	void	test_toDegree() const;
	// void	testExtractFromMatrix() const;

	// Vector3
	void	testVector3_everything() const;
	void	testVector3() const;
	void	testVector3_operatorEqual() const;
	void	testVector3_rotate() const;
	void	testVector3_translate() const;
	void	testVector3_add() const;
	void	testVector3_sub() const;
	void	testVector3_mult() const;
	void	testVector3_div() const;
	void	testVector3_magnitude() const;
	void	testVector3_operatorMinus() const; // in fact its for the sign minus, not the mathematical term operator
	void	testVector3_cross() const;
	void	testVector3_dot() const;
	
	// Rotation
	void	testRotation_everything() const;
	void	testRotation() const;
	void	testRotation_operatorEqual() const;
	void	testRotation_setAsRad() const;
	void	testRotation_setAsDegree() const;
	void	testRotation_setUnit() const;
	void	testRotation_toRadian() const;
	void	testRotation_toDegree() const;
	void	testRotation_isRadian() const;
	void	testRotation_isDegree() const;

	// Matrix4
	void	testMatrix4_everything() const;
	void	testMatrix4() const;
	void	testMatrix4_operatorEqual() const;
	void	testMatrix4_projectionMatrix() const;
	void	testMatrix4_viewMatrix() const;
	void	testMatrix4_modelMatrix() const;
	void	testMatrix4_scaleMatrix() const;
	void	testMatrix4_identity() const;
	void	testMatrix4_reset() const;
	void	testMatrix4_transpose() const;
	void	testMatrix4_printData() const;
	void	testMatrix4_updatePosValue() const;
	void	testMatrix4_div() const;
	void	testMatrix4_mult() const;
	void	testMatrix4_getData() const;
	void	testMatrix4_getOrder() const;

private:
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
	Math::Matrix4	mat4_identity;
	Math::Matrix4	mat4_clear;
	Math::Matrix4	mat4_0_to_15_RM;
	Math::Matrix4	mat4_0_to_15_RM_transposed;
	Math::Matrix4	mat4_15_to_0_CM;
	Math::Matrix4	mat4_only2_RM;
	Math::Matrix4	mat4_only8_RM;
	Math::Matrix4	mat4_scale_1_1_1_RM;
	Math::Matrix4	mat4_scale_4_5_6_RM;
};