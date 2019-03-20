#include "mathtests.hpp"

static bool	isEqual(float a, float b, int ulp) {
	return ((std::abs(a - b) <= std::numeric_limits<float>::epsilon() * std::abs(a + b) * ulp)
		|| (std::abs(a + b) < std::numeric_limits<float>::min()));
}
/********************************************************************/

void	MathTests::setUp() {
	this->values.degree125 = 125.0f;
	this->values.radian2_18166 = 2.18166f;
}

void	MathTests::tearDown() {
}

void	MathTests::testEverything() {
	this->testToRadian();
	this->testToDegree();
}

void	MathTests::testToRadian() {
	TEST(isEqual(Math::toRadian(values.degree125), values.radian2_18166, ULP));
	// TEST(Math::toRadian(values.degree125) == values.radian2_18166);
}
void	MathTests::testToDegree() {
	TEST(isEqual(Math::toDegree(values.radian2_18166), values.degree125, ULP));
	// TEST(Math::toDegree(values.radian2_18166) == values.degree125);
}