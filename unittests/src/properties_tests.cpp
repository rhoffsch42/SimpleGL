#include "properties_tests.hpp"

void	UnitTests::PropertiesTests::setUp() {
	vA_0_0_0.x = 0;				vA_0_0_0.y = 0;				vA_0_0_0.z = 0;
	vB_1_2_3.x = 1;				vB_1_2_3.y = 2;				vB_1_2_3.z = 3;
	pos_0_0_0.x = 0;			pos_0_0_0.y = 0;			pos_0_0_0.z = 0;
	pos_6_12_0.x = 6;			pos_6_12_0.y = 12;			pos_6_12_0.z = 0;
	pos_7_14_3.x = 7;			pos_7_14_3.y = 14;			pos_7_14_3.z = 3;
	rot_0_0_0_d.x = 0;			rot_0_0_0_d.y = 0;			rot_0_0_0_d.z = 0;		rot_0_0_0_d._unit = ROT_DEG;
	rot_10_15_45_d.x = 10;		rot_10_15_45_d.y = 15;		rot_10_15_45_d.z = 45;	rot_10_15_45_d._unit = ROT_DEG;
	rot_1_2_4_r.x = 1;			rot_1_2_4_r.y = 2;			rot_1_2_4_r.z = 4;		rot_1_2_4_r._unit = ROT_RAD;
	scale_1_1_1.x = 1;			scale_1_1_1.y = 1;			scale_1_1_1.z = 1;
	scale_2_3_4.x = 2;			scale_2_3_4.y = 3;			scale_2_3_4.z = 4;
	scale_3_5_7.x = 3;			scale_3_5_7.y = 5;			scale_3_5_7.z = 7;

	ppA._matrixChanged = false;
	for (int i = 0; i < 16; i++) {
		ppA._matrix.e[i] = i;
	}
	ppA._matrixUpdated = true;
	ppA._pos.x = 4;				ppA._pos.y = 9;					ppA._pos.z = 1;
	ppA._rot.x = 5;				ppA._rot.y = -9;				ppA._rot.z = 51;	ppA._rot._unit = ROT_RAD;
	ppA._scale.x = 2;			ppA._scale.y = 4;				ppA._scale.z = 1;
}
void	UnitTests::PropertiesTests::tearDown() {
}
void	UnitTests::PropertiesTests::testEverything() const {
	// UnitTests::streamTests << "pp tests" << std::endl;
	testProperties();
	testProperties_operatorEqual();
	testProperties_updateMatrix();
	testProperties_translate();
	testProperties_rotate();
	testProperties_enlarge();
	testProperties_setPos();
	testProperties_setRot();
	testProperties_setScale();
	testProperties_getMatrix();
	testProperties_getPos();
	testProperties_getRot();
	testProperties_getScale();
}

void	UnitTests::PropertiesTests::testProperties() const {
	Properties		pp1;

	TEST("Properties()", pp1._matrixChanged == true);
	TEST("Properties()", pp1._matrixUpdated == false);
	TEST("Properties()", UnitTests::MathTests::isEqualVector3(&pp1._pos, &pos_0_0_0));
	TEST("Properties()", UnitTests::MathTests::isEqualVector3(&pp1._rot, &rot_0_0_0_d));
	TEST("Properties()", UnitTests::MathTests::isEqualVector3(&pp1._scale, &scale_1_1_1));
}
void	UnitTests::PropertiesTests::testProperties_operatorEqual() const {
	Properties		pp2 = ppA;

	TEST("Properties operator=", pp2._matrixChanged == true);
	TEST("Properties operator=", pp2._matrixUpdated == false);
	TEST("Properties operator=", UnitTests::MathTests::isEqualVector3(&pp2._pos, &ppA._pos));
	TEST("Properties operator=", UnitTests::MathTests::isEqualVector3(&pp2._rot, &ppA._rot));
	TEST("Properties operator=", UnitTests::MathTests::isEqualVector3(&pp2._scale, &ppA._scale));
}
void	UnitTests::PropertiesTests::testProperties_updateMatrix() const {
	NOT_TESTED("Properties updateMatrix", " : todo", UnitTests::test_error);
}
void	UnitTests::PropertiesTests::testProperties_translate() const {
	Properties pp3 = ppA;
	bool same_as_before = pp3._matrixUpdated;

	pp3.translate(0, 0, 0);
	TEST("Properties translate(0, 0, 0)", UnitTests::MathTests::isEqualVector3(&pp3._pos, &ppA._pos));
	pp3.translate(2, 3, -1);
	TEST("Properties translate(2, 3, -1)", UnitTests::MathTests::isEqualVector3(&pp3._pos, &pos_6_12_0));
	pp3.translate(vA_0_0_0);
	TEST("Properties translate(vA_0_0_0)", UnitTests::MathTests::isEqualVector3(&pp3._pos, &pos_6_12_0));
	pp3.translate(vB_1_2_3);
	TEST("Properties translate(vB_1_2_3)", UnitTests::MathTests::isEqualVector3(&pp3._pos, &pos_7_14_3));
	TEST("Properties translate", pp3._matrixChanged == true);
	TEST("Properties translate", pp3._matrixUpdated == same_as_before);
}
void	UnitTests::PropertiesTests::testProperties_rotate() const {
	NOT_TESTED("Properties rotate", " : todo", UnitTests::test_error);
}
void	UnitTests::PropertiesTests::testProperties_enlarge() const {
	Properties pp4 = ppA;
	pp4._scale = scale_1_1_1;

	pp4.enlarge(1, 2, 3);
	TEST("Properties enlarge(1, 2, 3)", UnitTests::MathTests::isEqualVector3(&pp4._scale, &scale_2_3_4));
	pp4.enlarge(vB_1_2_3);
	TEST("Properties enlarge(vB_1_2_3)", UnitTests::MathTests::isEqualVector3(&pp4._scale, &scale_3_5_7));
}
void	UnitTests::PropertiesTests::testProperties_setPos() const {
	Properties pp5;

	pp5.setPos(1, 2, 3);
	TEST("Properties setPos(1, 2, 3)", UnitTests::MathTests::isEqualVector3(&pp5._pos, &vB_1_2_3));
	pp5.setPos(pos_6_12_0);
	TEST("Properties setPos(pos_6_12_0)", UnitTests::MathTests::isEqualVector3(&pp5._pos, &pos_6_12_0));
}
void	UnitTests::PropertiesTests::testProperties_setRot() const {
	Properties pp6;

	pp6.setRot(10, 15, 45);
	TEST("Properties setRot(10, 15, 45)", UnitTests::MathTests::isEqualVector3(&pp6._rot, &rot_10_15_45_d));
	TEST("Properties setRot(10, 15, 45)", pp6._rot._unit == ROT_DEG);
	pp6.setRot(rot_1_2_4_r);
	TEST("Properties setRot(rot_1_2_4_r)", UnitTests::MathTests::isEqualVector3(&pp6._rot, &rot_1_2_4_r));
	TEST("Properties setRot(rot_1_2_4_r)", pp6._rot._unit == ROT_RAD);
}
void	UnitTests::PropertiesTests::testProperties_setScale() const {
	Properties pp7;

	pp7.setScale(2, 3, 4);
	TEST("Properties setScale(2, 3, 4)", UnitTests::MathTests::isEqualVector3(&pp7._scale, &scale_2_3_4));
}
void	UnitTests::PropertiesTests::testProperties_getMatrix() const {
	Math::Matrix4&	mat1 = ppA.getMatrix();
	TEST("Properties getMatrix", UnitTests::MathTests::isEqualMat4(&mat1, &ppA._matrix));
	TEST("Properties getMatrix", &mat1 == &ppA._matrix);
}
void	UnitTests::PropertiesTests::testProperties_getPos() const {
	Math::Vector3 v8 = ppA.getPos();
	TEST("Properties getPos", UnitTests::MathTests::isEqualVector3(&v8, &ppA._pos));
	TEST("Properties getPos", &v8 != &ppA._pos);
}
void	UnitTests::PropertiesTests::testProperties_getRot() const {
	Math::Rotation v9 = ppA.getRot();
	TEST("Properties getRot", UnitTests::MathTests::isEqualVector3(&v9, &ppA._rot));
	TEST("Properties getRot", v9._unit == ppA._rot._unit);
	TEST("Properties getRot", &v9 != &ppA._rot);
}
void	UnitTests::PropertiesTests::testProperties_getScale() const {
	Math::Vector3 v10 = ppA.getScale();
	TEST("Properties getScale", UnitTests::MathTests::isEqualVector3(&v10, &ppA._scale));
	TEST("Properties getScale", &v10 != &ppA._scale);
}
