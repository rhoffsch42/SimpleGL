#pragma once

#include "unittests.hpp"
#include "properties.hpp"
#include "math_tests.hpp"

class UnitTests::PropertiesTests {
public:
	void	setUp();
	void	tearDown();
	void	testEverything() const;

	void	testProperties() const;
	void	testProperties_operatorEqual() const;
	void	testProperties_updateMatrix() const;
	void	testProperties_translate() const;
	void	testProperties_rotate() const;
	void	testProperties_enlarge() const;
	void	testProperties_setPos() const;
	void	testProperties_setRot() const;
	void	testProperties_setScale() const;
	void	testProperties_getMatrix() const;
	void	testProperties_getPos() const;
	void	testProperties_getRot() const;
	void	testProperties_getScale() const;
	void	testProperties_getCenterOffset() const;
	void	testProperties_getCenteredPos() const;
	void	testProperties_center() const;
private:
	Math::Vector3	vA_0_0_0;
	Math::Vector3	vB_1_2_3;
	Math::Vector3	pos_0_0_0;
	Math::Vector3	pos_6_12_0;
	Math::Vector3	pos_7_14_3;
	Math::Rotation	rot_0_0_0_d;
	Math::Rotation	rot_10_15_45_d;
	Math::Rotation	rot_1_2_4_r;
	Math::Vector3	scale_1_1_1;
	Math::Vector3	scale_2_3_4;
	Math::Vector3	scale_3_5_7;
	Properties		ppA;
};