#include "math_tests.hpp"

static bool	isEqualf(float a, float b, int ulp) {
	// std::cout << std::fixed << std::setprecision(20) << std::abs(a - b) << std::endl;
	// return (std::abs(a - b) <= 0.001f);
	return ((std::abs(a - b) <= std::numeric_limits<float>::epsilon() * std::abs(a + b) * ulp)
		|| (std::abs(a + b) < std::numeric_limits<float>::min()));
}

bool	UnitTests::MathTests::isEqualMat4(const Math::Matrix4 *m1, const Math::Matrix4 *m2) {
	const float	*elements1 = &(m1->e[0]);
	const float	*elements2 = &(m2->e[0]);

	if (m1->_order != m2->_order)
		return (false);
	for (size_t i = 0; i < 16; i++) {
		if (!isEqualf(elements1[i], elements2[i], ULP))
			return (false);
	}
	return (true);
}

bool	UnitTests::MathTests::isEqualVector3(const Math::Vector3 *v1, const Math::Vector3 *v2) {
	return (isEqualf(v1->x, v2->x, ULP) && isEqualf(v1->y, v2->y, ULP) && isEqualf(v1->z, v2->z, ULP));
}

void	printTrueFalse(bool val) {
	std::cout << (val ? "true" : "false") << std::endl;
}
/********************************************************************/

void	UnitTests::MathTests::setUp() {
	degree125 = 125.0f;
	radian2_18166 = 2.18166f;
	
	// Vector3
	coo_1_2_3.x = 1.0f;				coo_1_2_3.y = 2.0f;				coo_1_2_3.z = 3.0f;
	vecA_0_0_0.x = 0.0f;			vecA_0_0_0.y = 0.0f;			vecA_0_0_0.z = 0.0f;
	vecB_4_5_6.x = 4.0f;			vecB_4_5_6.y = 5.0f;			vecB_4_5_6.z = 6.0f;
	vecC_90_0_m45.x = -90.0f;		vecC_90_0_m45.y = 0.0f;			vecC_90_0_m45.z = -45.0f;
	vecD_3_4_5.x = 3.0f;			vecD_3_4_5.y = 4.0f;			vecD_3_4_5.z = 5.0f;
	vecE_1_1_1.x = 1.0f;			vecE_1_1_1.y = 1.0f;			vecE_1_1_1.z = 1.0f;
	vecF_m3_6_m3.x = -3.0f;			vecF_m3_6_m3.y = 6.0f;			vecF_m3_6_m3.z = -3.0f;
	magnitude_18 = 18.0f;
	dot_122 = 122.0f;
	
	// Rotation
	rotA_0_0_0_d.x = 0.0f;			rotA_0_0_0_d.y = 0.0f;			rotA_0_0_0_d.z = 0.0f;			rotA_0_0_0_d._unit = ROT_DEG;
	rotB_1_2_3_d.x = 1.0f;			rotB_1_2_3_d.y = 2.0f;			rotB_1_2_3_d.z = 3.0f;			rotB_1_2_3_d._unit = ROT_DEG;
	rotC_r.x = 3.14159f;			rotC_r.y = 0.0f;				rotC_r.z = 1.74533f;			rotC_r._unit = ROT_RAD; // 180, 0, 100 deg
	rotD_180_0_100_d.x = 180.0f;	rotD_180_0_100_d.y = 0.0f;		rotD_180_0_100_d.z = 100.0f;	rotD_180_0_100_d._unit = ROT_DEG;
	
	// Matrix4
	mat4_identity._order = ROW_MAJOR;
	float	*m1 = &(mat4_identity.e[0]);
	m1[0] = m1[5] = m1[10] = m1[15] = 1.0f;
	m1[1] = m1[2] = m1[3] = m1[4] = m1[6] = m1[7] = m1[8] = m1[9] = m1[11] = m1[12] = m1[13] = m1[14] = 0.0f;
	
	mat4_clear._order = ROW_MAJOR;
	float	*m2 = &(mat4_clear.e[0]);
	m2[0] = m2[1] = m2[2] = m2[3] = m2[4] = m2[5] = m2[6] = m2[7] = m2[8] = m2[9] = m2[10] = m2[11] = m2[12] = m2[13] = m2[14] = m2[15] = 0.0f;
	
	mat4_0_to_15_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		mat4_0_to_15_RM.e[i] = i;
	}

	mat4_0_to_15_RM_transposed._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		mat4_0_to_15_RM_transposed.e[i] = i * 4 % 16 + (i * 4 / 16);
	}

	mat4_15_to_0_CM._order = COLUMN_MAJOR;
	for (int i = 0; i < 16; i++) {
		mat4_15_to_0_CM.e[i] = 15 - i;
	}

	mat4_only2_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		mat4_only2_RM.e[i] = 2.0f;
	}

	mat4_only8_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		mat4_only8_RM.e[i] = 8.0f;
	}

	mat4_scale_1_1_1_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		mat4_scale_1_1_1_RM.e[i] = 0.0f;
	}
	mat4_scale_1_1_1_RM.tab[0][0] = 1.0f;	mat4_scale_1_1_1_RM.tab[1][1] = 1.0f;	mat4_scale_1_1_1_RM.tab[2][2] = 1.0f;
	mat4_scale_1_1_1_RM.tab[3][3] = 1.0f;

	mat4_scale_4_5_6_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		mat4_scale_4_5_6_RM.e[i] = 0.0f;
	}
	mat4_scale_4_5_6_RM.tab[0][0] = 4.0f;	mat4_scale_4_5_6_RM.tab[1][1] = 5.0f;	mat4_scale_4_5_6_RM.tab[2][2] = 6.0f;
	mat4_scale_4_5_6_RM.tab[3][3] = 1.0f;

}
void	UnitTests::MathTests::tearDown() {
}
void	UnitTests::MathTests::testEverything() const {
#if TEST_MATH_UTIL
	test_toRadian();
	test_toDegree();
	UnitTests::streamTests << std::endl;
#endif
#if TEST_MATH_VECTOR3
	testVector3_everything();
	UnitTests::streamTests << std::endl;
#endif
#if TEST_MATH_ROTATION
	testRotation_everything();
	UnitTests::streamTests << std::endl;
#endif
#if TEST_MATH_MATRIX4
	testMatrix4_everything();
#endif
}

#if TEST_MATH_UTIL
	void	UnitTests::MathTests::test_toRadian() const {
		TEST("Math::toRadian(deg)", isEqualf(Math::toRadian(degree125), radian2_18166, ULP));
	}
	void	UnitTests::MathTests::test_toDegree() const {
		TEST("Math::toDegree(rad)", isEqualf(Math::toDegree(radian2_18166), degree125, ULP));
	}
#endif

#if TEST_MATH_VECTOR3
	void	UnitTests::MathTests::testVector3_everything() const {
		testVector3();
		// other tests		
		testVector3_operatorEqual();
		testVector3_rotate();
		testVector3_translate();
		testVector3_add();
		testVector3_sub();
		testVector3_mult();
		testVector3_div();
		testVector3_magnitude();
		testVector3_operatorMinus();
		testVector3_cross();
		testVector3_dot();
	}
	void	UnitTests::MathTests::testVector3() const {
		Math::Vector3	v1;
		TEST("Math::Vector3()", isEqualVector3(&v1, &vecA_0_0_0));

		Math::Vector3	v2(coo_1_2_3.x, coo_1_2_3.y, coo_1_2_3.z);
		TEST("Math::Vector3(x, y, z)", isEqualVector3(&v2, &coo_1_2_3));

		Math::Vector3	v3(v2);
		TEST("Math::Vector3(vec)", isEqualVector3(&v3, &v2));
	}
	void	UnitTests::MathTests::testVector3_operatorEqual() const {
		Math::Vector3	v4 = vecB_4_5_6;
		TEST("Math::Vector3 operator=", isEqualVector3(&v4, &vecB_4_5_6));
	}
	void	UnitTests::MathTests::testVector3_rotate() const {

		Math::Vector3	v5(45, 0, -90);
		v5.rotate(90, 90, -90, ROT_WAY);
		TEST("Math::Vector3 rotate", isEqualVector3(&v5, &vecC_90_0_m45));
		NOT_TESTED("Math::Vector3 rotate(rot, ROT_WAY)", "", UnitTests::test_error);
		NOT_TESTED("Math::Vector3 ZYXrotate(rot, ROT_WAY)", "", UnitTests::test_error);
	}
	void	UnitTests::MathTests::testVector3_add() const {
		Math::Vector3	v8(1, 1, 1);
		v8.add(3, 4, 5);
		TEST("Math::Vector3 add(x, y, z)", isEqualVector3(&v8, &vecB_4_5_6));

		Math::Vector3	v9(1, 1, 1);
		v9.add(vecD_3_4_5);
		TEST("Math::Vector3 add(vec)", isEqualVector3(&v9, &vecB_4_5_6));
	}
	void	UnitTests::MathTests::testVector3_sub() const {
		Math::Vector3	v10(5, 6, 7);
		v10.sub(4, 5, 6);
		TEST("Math::Vector3 sub(x, y, z)", isEqualVector3(&v10, &vecE_1_1_1));

		Math::Vector3	v11(5, 6, 7);
		v11.sub(vecB_4_5_6);
		TEST("Math::Vector3 sub(vec)", isEqualf(v11.x, vecE_1_1_1.x, ULP) &&
										isEqualf(v11.y, vecE_1_1_1.y, ULP) &&
										isEqualf(v11.z, vecE_1_1_1.z, ULP)
		);
		TEST("Math::Vector3 sub(vec)", isEqualVector3(&v11, &vecE_1_1_1));
	}
	void	UnitTests::MathTests::testVector3_mult() const {
		Math::Vector3	v12(2, 2.5f, 3);
		v12.mult(2);
		TEST("Math::Vector3 mult(coef)", isEqualVector3(&v12, &vecB_4_5_6));
	}
	void	UnitTests::MathTests::testVector3_div() const {
		Math::Vector3	v13(2, 2, 2);
		v13.div(2);
		TEST("Math::Vector3 div(coef)", isEqualVector3(&v13, &vecE_1_1_1));
	}
	void	UnitTests::MathTests::testVector3_len() const {
		Math::Vector3	v14(2, 16, 8);
		float	mag1 = v14.len();
		TEST("Math::Vector3 len()", isEqualf(mag1, magnitude_18, ULP));
	}
	void	UnitTests::MathTests::testVector3_operatorMinus() const {
		Math::Vector3	v15(-1, -1, -1);
		Math::Vector3	v16 = -v15;
		TEST("Math::Vector3 sign-", isEqualVector3(&v16, &vecE_1_1_1));
	}
	void	UnitTests::MathTests::testVector3_cross() const {
		Math::Vector3	v17(2, 3, 4);
		Math::Vector3	v18(5, 6, 7);
		Math::Vector3	v19 = Math::Vector3::cross(v17, v18);
		TEST("Math::Vector3 cross(v1, v2)", isEqualVector3(&v19, &vecF_m3_6_m3));
	}
	void	UnitTests::MathTests::testVector3_dot() const {
		Math::Vector3	v20(9, 2, 7);
		Math::Vector3	v21(4, 8, 10);
		float			dot = Math::Vector3::dot(v20, v21);
		TEST("Math::Vector3 dot(v1, v2)", isEqualf(dot, dot_122, ULP));
	}
#endif

#if TEST_MATH_ROTATION
	void	UnitTests::MathTests::testRotation_everything() const {
		// prerequisite
		testRotation_isRadian();
		testRotation_isDegree();

		testRotation();

		// other tests
		testRotation_operatorEqual();
		testRotation_setAsRad();
		testRotation_setAsDegree();
		testRotation_setUnit();
		testRotation_toRadian();
		testRotation_toDegree();
	}
	void	UnitTests::MathTests::testRotation() const {

		Math::Rotation	rot1;
		TEST("Math::Rotation()", isEqualVector3(&rot1, &rotA_0_0_0_d));
		TEST("Math::Rotation()", rot1.isDegree());

		Math::Rotation	rot2(1, 2, 3);
		TEST("Math::Rotation(x, y, z)", isEqualVector3(&rot2, &rotB_1_2_3_d));
		TEST("Math::Rotation(x, y, z)", rot2.isDegree());

		Math::Rotation	rot3(1, 2, 3, ROT_DEG);
		TEST("Math::Rotation(x, y, z, ROT_DEG)", isEqualVector3(&rot3, &rotB_1_2_3_d));
		TEST("Math::Rotation(x, y, z, ROT_DEG)", rot3.isDegree());

		Math::Rotation	rot4(3.14159f, 0, 1.74533f, ROT_RAD);	// 180, 0, 100
		TEST("Math::Rotation(x, y, z, ROT_RAD)", isEqualVector3(&rot4, &rotC_r));
		TEST("Math::Rotation(x, y, z, ROT_RAD)", rot4.isRadian());

	}
	void	UnitTests::MathTests::testRotation_operatorEqual() const {
		Math::Rotation	rot5 = rotC_r;	// 180, 0, 100
		TEST("Math::Rotation operator=", isEqualVector3(&rot5, &rotC_r));
		TEST("Math::Rotation operator=", rot5.isRadian());
	}
	void	UnitTests::MathTests::testRotation_setAsRad() const {
		Math::Rotation	rot6 = rotD_180_0_100_d;
		rot6.setAsRad();
		TEST("Math::Rotation setAsRad", isEqualVector3(&rot6, &rotC_r));
		TEST("Math::Rotation setAsRad", rot6.isRadian());
	}
	void	UnitTests::MathTests::testRotation_setAsDegree() const {
		Math::Rotation	rot7 = rotC_r;	// 180, 0, 100
		rot7.setAsDegree();
		TEST("Math::Rotation setAsDegree", isEqualVector3(&rot7, &rotD_180_0_100_d));
		TEST("Math::Rotation setAsDegree", rot7.isDegree());
	}
	void	UnitTests::MathTests::testRotation_setUnit() const {
		Math::Rotation	rotDegree = rotC_r;
		Math::Rotation	rotRadian = rotA_0_0_0_d;
		rotDegree.setUnit(ROT_DEG);
		rotRadian.setUnit(ROT_RAD);
		TEST("Math::Rotation SetUnit(ROT_DEG)", rotDegree.isDegree());
		TEST("Math::Rotation SetUnit(ROT_RAD)", rotRadian.isRadian());
	}
	void	UnitTests::MathTests::testRotation_toRadian() const {
		Math::Rotation		rot8 = rotD_180_0_100_d.toRadian();
		TEST("Math::Rotation toRadian", isEqualVector3(&rot8, &rotC_r));
		TEST("Math::Rotation toRadian", rot8.isRadian());
	}
	void	UnitTests::MathTests::testRotation_toDegree() const {
		Math::Rotation		rot9 = rotC_r.toDegree();	// 180 0 100 deg
		TEST("Math::Rotation toDegree", isEqualVector3(&rot9, &rotD_180_0_100_d));
		TEST("Math::Rotation toDegree", rot9.isDegree());
	}
	void	UnitTests::MathTests::testRotation_isRadian() const {
		Math::Rotation	rot0;
		TEST("Math::Rotation isRadian()", !rot0.isRadian());

	}
	void	UnitTests::MathTests::testRotation_isDegree() const {
		Math::Rotation	rot0;
		TEST("Math::Rotation isDegree()", rot0.isDegree());
	}
#endif

#if TEST_MATH_MATRIX4
	void	UnitTests::MathTests::testMatrix4_everything() const {
		// prerequisite
		testMatrix4_identity();
		testMatrix4_reset();

		//ructors
		testMatrix4();

		// other tests
		testMatrix4_operatorEqual();
		testMatrix4_projectionMatrix();
		testMatrix4_viewMatrix();
		testMatrix4_modelMatrix();
		testMatrix4_scaleMatrix();
		testMatrix4_transpose();
		testMatrix4_printData();
		testMatrix4_updatePosValue();
		testMatrix4_div();
		testMatrix4_mult();
		testMatrix4_getData();
		testMatrix4_getOrder();
	}
	void	UnitTests::MathTests::testMatrix4() const {
		Math::Matrix4	m3;	
		TEST("Math::Matrix4 Matrix4()", UnitTests::MathTests::isEqualMat4(&m3, &(mat4_identity)));
	}
	void	UnitTests::MathTests::testMatrix4_operatorEqual() const {
		Math::Matrix4	m4 = mat4_0_to_15_RM;
		Math::Matrix4	m5 = mat4_15_to_0_CM;
		TEST("Math::Matrix4 operator=", UnitTests::MathTests::isEqualMat4(&m4, &(mat4_0_to_15_RM)));
		TEST("Math::Matrix4 operator=", UnitTests::MathTests::isEqualMat4(&m5, &(mat4_15_to_0_CM)));
	}
	void	UnitTests::MathTests::testMatrix4_projectionMatrix() const {
		Math::Matrix4	m6;
		float	fovRadian = 0;
		float	farValue = 0;
		float	nearValue = 0;
		float	win_width = 0;
		float	win_height = 0;
		m6.projectionMatrix(fovRadian, farValue, nearValue, win_width, win_height);
		NOT_TESTED("Math::Matrix4 projectionMatrix", " : ca peut dependre du rendu que l'on veut", UnitTests::test_error);
		/*
			see math.cpp
			la matrice peut changer selon qu'on veux etirer le rendu dans la fenetre, ou bien toujours garder un aspect ratio de 1/1.
			+ voir les diff avec https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix
		*/		
	}
	void	UnitTests::MathTests::testMatrix4_viewMatrix() const {
		Math::Matrix4	m7;
		m7.viewMatrix(vecD_3_4_5, rotD_180_0_100_d);
		NOT_TESTED("Math::Matrix4 viewMatrix", " : todo", UnitTests::test_error);
	}
	void	UnitTests::MathTests::testMatrix4_modelMatrix() const {
		Math::Matrix4	m8;
		m8.modelMatrix(vecD_3_4_5, rotD_180_0_100_d, vecE_1_1_1);
		NOT_TESTED("Math::Matrix4 modelMatrix", " : todo", UnitTests::test_error);
	}
	void	UnitTests::MathTests::testMatrix4_scaleMatrix() const {
		Math::Matrix4	m9;
		m9.scaleMatrix(vecE_1_1_1);
		TEST("Math::Matrix4 scale(1, 1, 1)", UnitTests::MathTests::isEqualMat4(&m9, &(mat4_scale_1_1_1_RM)));
		m9.scaleMatrix(vecB_4_5_6);
		TEST("Math::Matrix4 scale(4, 5, 6)", UnitTests::MathTests::isEqualMat4(&m9, &(mat4_scale_4_5_6_RM)));
	}
	void	UnitTests::MathTests::testMatrix4_identity() const {
		Math::Matrix4	m1;
		for (int i = 0; i < 15; i++) {
			m1.e[i] = i;
		}
		m1.identity();
		m1._order = ROW_MAJOR;
		TEST("Math::Matrix4 identity()", UnitTests::MathTests::isEqualMat4(&m1, &(mat4_identity) ) );
	}
	void	UnitTests::MathTests::testMatrix4_reset() const {
		Math::Matrix4	m2;
		for (int i = 0; i < 15; i++) {
			m2.e[i] = i;
		}
		m2._order = ROW_MAJOR;
		m2.reset();
		TEST("Math::Matrix4 reset()", UnitTests::MathTests::isEqualMat4(&m2, &(mat4_clear)));
	}
	void	UnitTests::MathTests::testMatrix4_transpose() const {
		Math::Matrix4	m10 = mat4_0_to_15_RM;
		m10.transpose();
		TEST("Math::Matrix4 transpose()", UnitTests::MathTests::isEqualMat4(&m10, &(mat4_0_to_15_RM_transposed)));
	}
	void	UnitTests::MathTests::testMatrix4_printData() const {
		NOT_TESTED("Math::Matrix4 printData()", " : not needed", UnitTests::test_error);
	}
	void	UnitTests::MathTests::testMatrix4_updatePosValue() const {
		NOT_TESTED("Math::Matrix4 updatePosValue", " : todo", UnitTests::test_error);
	}
	void	UnitTests::MathTests::testMatrix4_div() const {
		Math::Matrix4	m13 = mat4_only8_RM;
		m13.div(4.0f);
		TEST("Math::Matrix4 div()", UnitTests::MathTests::isEqualMat4(&m13, &(mat4_only2_RM)));
	}
	void	UnitTests::MathTests::testMatrix4_mult() const {
		Math::Matrix4	m14 = mat4_only2_RM;
		m14.mult(4.0f);
		TEST("Math::Matrix4 mult()", UnitTests::MathTests::isEqualMat4(&m14, &(mat4_only8_RM)));
	}
	void	UnitTests::MathTests::testMatrix4_getData() const {
		float	*data = mat4_0_to_15_RM.getData();
		TEST("Math::Matrix4 getData()", data == &(mat4_0_to_15_RM.e[0]));
		
	}
	void	UnitTests::MathTests::testMatrix4_getOrder() const {
		uint8_t		order = mat4_0_to_15_RM.getOrder();
		TEST("Math::Matrix4 getOrder()", order == mat4_0_to_15_RM._order);
	}
#endif