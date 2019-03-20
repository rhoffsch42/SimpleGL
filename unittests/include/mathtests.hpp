#pragma once

#include "unittests.hpp"
#include "math.hpp"
#include <cmath>
#define ULP	4

typedef struct	s_var {
	float	degree125;
	float	radian2_18166;
}				t_var;

class MathTests {
public:
	t_var	values;//private?

	void	setUp();
	void	tearDown();
	void	testEverything();

	void	testToRadian();
	void	testToDegree();
	// void	testExtractFromMatrix();

/*
	// Vector3
	void	testVector3();
	void	testVector3operatorEqual();
	void	testVector3rotate();
	void	testVector3ZYXrotate();
	void	testVector3translate();
	void	testVector3add();
	void	testVector3sub();
	void	testVector3mult();
	void	testVector3div();
	void	testVector3magnitude();
	void	testVector3operatorMinus();
	void	testVector3cross();
	void	testVector3dot();
	
	// Rotation
	void	testRotation();

	// Matrix4
	void	testMatrix4();
*/
};