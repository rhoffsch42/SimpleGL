#include "mathtests.hpp"


static bool	isEqualf(float a, float b, int ulp) {
	// std::cout << std::fixed << std::setprecision(20) << std::abs(a - b) << std::endl;
	// return (std::abs(a - b) <= 0.001f);
	return ((std::abs(a - b) <= std::numeric_limits<float>::epsilon() * std::abs(a + b) * ulp)
		|| (std::abs(a + b) < std::numeric_limits<float>::min()));
}

bool	isEqualMat4(FreeAccess<Math::Matrix4> *m1, FreeAccess<Math::Matrix4> *m2) {
	float	*elements1 = &(m1->e[0]);
	float	*elements2 = &(m2->e[0]);

	if (m1->_order != m2->_order)
		return (false);
	for (size_t i = 0; i < 16; i++) {
		if (!isEqualf(elements1[i], elements2[i], ULP))
			return (false);
	}
	return (true);
}

void	printTrueFalse(bool val) {
	std::cout << (val ? "true" : "false") << std::endl;
}
/********************************************************************/

void	MathTests::setUp() {
	v.degree125 = 125.0f;
	v.radian2_18166 = 2.18166f;
	
	// Vector3
	v.coo_1_2_3[0] = 1.0f;			v.coo_1_2_3[1] = 2.0f;			v.coo_1_2_3[2] = 3.0f;
	v.vecA_0_0_0.x = 0.0f;			v.vecA_0_0_0.y = 0.0f;			v.vecA_0_0_0.z = 0.0f;
	v.vecB_4_5_6.x = 4.0f;			v.vecB_4_5_6.y = 5.0f;			v.vecB_4_5_6.z = 6.0f;
	v.vecC_90_0_m45.x = -90.0f;		v.vecC_90_0_m45.y = 0.0f;		v.vecC_90_0_m45.z = -45.0f;
	v.vecD_3_4_5.x = 3.0f;			v.vecD_3_4_5.y = 4.0f;			v.vecD_3_4_5.z = 5.0f;
	v.vecE_1_1_1.x = 1.0f;			v.vecE_1_1_1.y = 1.0f;			v.vecE_1_1_1.z = 1.0f;
	v.vecF_m3_6_m3.x = -3.0f;		v.vecF_m3_6_m3.y = 6.0f;		v.vecF_m3_6_m3.z = -3.0f;
	v.magnitude_18 = 18.0f;
	v.dot_122 = 122.0f;
	
	// Rotation
	v.rotA_0_0_0_d.x = 0.0f;		v.rotA_0_0_0_d.y = 0.0f;		v.rotA_0_0_0_d.z = 0.0f;		v.rotA_0_0_0_d.setUnit(ROT_DEG);
	v.rotB_1_2_3_d.x = 1.0f;		v.rotB_1_2_3_d.y = 2.0f;		v.rotB_1_2_3_d.z = 3.0f;		v.rotB_1_2_3_d.setUnit(ROT_DEG);
	v.rotC_r.x = 3.14159f;			v.rotC_r.y = 0.0f;				v.rotC_r.z = 1.74533f;			v.rotC_r.setUnit(ROT_RAD); // 180, 0, 100 deg
	v.rotD_180_0_100_d.x = 180.0f;	v.rotD_180_0_100_d.y = 0.0f;	v.rotD_180_0_100_d.z = 100.0f;	v.rotD_180_0_100_d.setUnit(ROT_DEG);
	
	// Matrix4
	v.mat4_identity._order = ROW_MAJOR;
	float	*m1 = &(v.mat4_identity.e[0]);
	m1[0] = m1[5] = m1[10] = m1[15] = 1.0f;
	m1[1] = m1[2] = m1[3] = m1[4] = m1[6] = m1[7] = m1[8] = m1[9] = m1[11] = m1[12] = m1[13] = m1[14] = 0.0f;
	
	v.mat4_clear._order = ROW_MAJOR;
	float	*m2 = &(v.mat4_clear.e[0]);
	m2[0] = m2[1] = m2[2] = m2[3] = m2[4] = m2[5] = m2[6] = m2[7] = m2[8] = m2[9] = m2[10] = m2[11] = m2[12] = m2[13] = m2[14] = m2[15] = 0.0f;
	
	v.mat4_0_to_15_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		v.mat4_0_to_15_RM.e[i] = i;
	}

	v.mat4_0_to_15_RM_transposed._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		v.mat4_0_to_15_RM_transposed.e[i] = i * 4 % 16 + (i * 4 / 16);
	}

	v.mat4_15_to_0_CM._order = COLUMN_MAJOR;
	for (int i = 0; i < 16; i++) {
		v.mat4_15_to_0_CM.e[i] = 15 - i;
	}

	v.mat4_only2_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		v.mat4_only2_RM.e[i] = 2.0f;
	}

	v.mat4_only8_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		v.mat4_only8_RM.e[i] = 8.0f;
	}

	v.mat4_scale_1_1_1_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		v.mat4_scale_1_1_1_RM.e[i] = 0.0f;
	}
	v.mat4_scale_1_1_1_RM.tab[0][0] = 1.0f;	v.mat4_scale_1_1_1_RM.tab[1][1] = 1.0f;	v.mat4_scale_1_1_1_RM.tab[2][2] = 1.0f;
	v.mat4_scale_1_1_1_RM.tab[3][3] = 1.0f;

	v.mat4_scale_4_5_6_RM._order = ROW_MAJOR;
	for (int i = 0; i < 16; i++) {
		v.mat4_scale_4_5_6_RM.e[i] = 0.0f;
	}
	v.mat4_scale_4_5_6_RM.tab[0][0] = 4.0f;	v.mat4_scale_4_5_6_RM.tab[1][1] = 5.0f;	v.mat4_scale_4_5_6_RM.tab[2][2] = 6.0f;
	v.mat4_scale_4_5_6_RM.tab[3][3] = 1.0f;


}
void	MathTests::tearDown() {
}
void	MathTests::testEverything() {
#if TEST_MATH_UTIL
	test_toRadian();
	test_toDegree();
	std::cout << std::endl;
#endif
#if TEST_MATH_VECTOR3
	testVector3_everything();
	std::cout << std::endl;
#endif
#if TEST_MATH_ROTATION
	testRotation_everything();
	std::cout << std::endl;
#endif
#if TEST_MATH_MATRIX4
	testMatrix4_everything();
	std::cout << std::endl;
#endif
}

#if TEST_MATH_UTIL
	void	MathTests::test_toRadian() {
		TEST("Math::toRadian(deg)", isEqualf(Math::toRadian(v.degree125), v.radian2_18166, ULP));
	}
	void	MathTests::test_toDegree() {
		TEST("Math::toDegree(rad)", isEqualf(Math::toDegree(v.radian2_18166), v.degree125, ULP));
	}
#endif

#if TEST_MATH_VECTOR3
	void	MathTests::testVector3_everything() {
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
	void	MathTests::testVector3() {
		Math::Vector3	v1;
		TEST("Math::Vector3()", isEqualf(v1.x, 0, ULP) &&
								isEqualf(v1.y, 0, ULP) &&
								isEqualf(v1.z, 0, ULP)
		);
		Math::Vector3	v2(v.coo_1_2_3[0], v.coo_1_2_3[1], v.coo_1_2_3[2]);
		TEST("Math::Vector3(x, y, z)", isEqualf(v2.x, v.coo_1_2_3[0], ULP) &&
										isEqualf(v2.y, v.coo_1_2_3[1], ULP) &&
										isEqualf(v2.z, v.coo_1_2_3[2], ULP)
		);
		Math::Vector3	v3(v2);
		TEST("Math::Vector3(vec)", isEqualf(v3.x, v2.x, ULP) &&
										isEqualf(v3.y, v2.y, ULP) &&
										isEqualf(v3.z, v2.z, ULP)
		);
	}
	void	MathTests::testVector3_operatorEqual() {
		Math::Vector3	v4 = v.vecB_4_5_6;
		TEST("Math::Vector3 operator=", isEqualf(v4.x, v.vecB_4_5_6.x, ULP) &&
										isEqualf(v4.y, v.vecB_4_5_6.y, ULP) &&
										isEqualf(v4.z, v.vecB_4_5_6.z, ULP)
		);
	}
	void	MathTests::testVector3_rotate() {

		Math::Vector3	v5(45, 0, -90);
		v5.rotate(90, 90, -90, ROT_WAY);
		TEST("Math::Vector3 rotate", isEqualf(v5.x, v.vecC_90_0_m45.x, ULP) &&
										isEqualf(v5.y, v.vecC_90_0_m45.y, ULP) &&
										isEqualf(v5.z, v.vecC_90_0_m45.z, ULP)
		);
		NOT_TESTED("Math::Vector3 rotate(rot, ROT_WAY", "");
		NOT_TESTED("Math::Vector3 ZYXrotate(rot, ROT_WAY", "");
	}
	void	MathTests::testVector3_translate() {
		Math::Vector3	v6(1, 1, 1);
		v6.translate(3, 4, 5);
		TEST("Math::Vector3 translate(x, y, z)", isEqualf(v6.x, v.vecB_4_5_6.x, ULP) &&
												isEqualf(v6.y, v.vecB_4_5_6.y, ULP) &&
												isEqualf(v6.z, v.vecB_4_5_6.z, ULP)
		);
		Math::Vector3	v7(1, 1, 1);
		v7.translate(v.vecD_3_4_5);
		TEST("Math::Vector3 translate(vec)", isEqualf(v7.x, v.vecB_4_5_6.x, ULP) &&
											isEqualf(v7.y, v.vecB_4_5_6.y, ULP) &&
											isEqualf(v7.z, v.vecB_4_5_6.z, ULP)
		);
	}
	void	MathTests::testVector3_add() {
		Math::Vector3	v8(1, 1, 1);
		v8.add(3, 4, 5);
		TEST("Math::Vector3 add(x, y, z)", isEqualf(v8.x, v.vecB_4_5_6.x, ULP) &&
											isEqualf(v8.y, v.vecB_4_5_6.y, ULP) &&
											isEqualf(v8.z, v.vecB_4_5_6.z, ULP)
		);
		Math::Vector3	v9(1, 1, 1);
		v9.add(v.vecD_3_4_5);
		TEST("Math::Vector3 add(vec)", isEqualf(v9.x, v.vecB_4_5_6.x, ULP) &&
										isEqualf(v9.y, v.vecB_4_5_6.y, ULP) &&
										isEqualf(v9.z, v.vecB_4_5_6.z, ULP)
		);
	}
	void	MathTests::testVector3_sub() {
		Math::Vector3	v10(5, 6, 7);
		v10.sub(4, 5, 6);
		TEST("Math::Vector3 sub(x, y, z)", isEqualf(v10.x, v.vecE_1_1_1.x, ULP) &&
											isEqualf(v10.y, v.vecE_1_1_1.y, ULP) &&
											isEqualf(v10.z, v.vecE_1_1_1.z, ULP)
		);
		Math::Vector3	v11(5, 6, 7);
		v11.sub(v.vecB_4_5_6);
		TEST("Math::Vector3 sub(vec)", isEqualf(v11.x, v.vecE_1_1_1.x, ULP) &&
										isEqualf(v11.y, v.vecE_1_1_1.y, ULP) &&
										isEqualf(v11.z, v.vecE_1_1_1.z, ULP)
		);
	}
	void	MathTests::testVector3_mult() {
		Math::Vector3	v12(2, 2.5f, 3);
		v12.mult(2);
		TEST("Math::Vector3 mult(coef)", isEqualf(v12.x, v.vecB_4_5_6.x, ULP) &&
										isEqualf(v12.y, v.vecB_4_5_6.y, ULP) &&
										isEqualf(v12.z, v.vecB_4_5_6.z, ULP)
		);
	}
	void	MathTests::testVector3_div() {
		Math::Vector3	v13(2, 2, 2);
		v13.div(2);
		TEST("Math::Vector3 div(coef)", isEqualf(v13.x, v.vecE_1_1_1.x, ULP) &&
										isEqualf(v13.y, v.vecE_1_1_1.y, ULP) &&
										isEqualf(v13.z, v.vecE_1_1_1.z, ULP)
		);
	}
	void	MathTests::testVector3_magnitude() {
		Math::Vector3	v14(2, 16, 8);
		float	mag1 = v14.magnitude();
		TEST("Math::Vector3 magnitude()", isEqualf(mag1, v.magnitude_18, ULP));
	}
	void	MathTests::testVector3_operatorMinus() {
		Math::Vector3	v15(-1, -1, -1);
		Math::Vector3	v16 = -v15;
		TEST("Math::Vector3 sign-", isEqualf(v16.x, v.vecE_1_1_1.x, ULP) &&
									isEqualf(v16.y, v.vecE_1_1_1.y, ULP) &&
									isEqualf(v16.z, v.vecE_1_1_1.z, ULP)
		);
	}
	void	MathTests::testVector3_cross() {
		Math::Vector3	v17(2, 3, 4);
		Math::Vector3	v18(5, 6, 7);
		Math::Vector3	v19 = Math::Vector3::cross(v17, v18);
		TEST("Math::Vector3 cross(v1, v2)", isEqualf(v19.x, v.vecF_m3_6_m3.x, ULP) &&
											isEqualf(v19.y, v.vecF_m3_6_m3.y, ULP) &&
											isEqualf(v19.z, v.vecF_m3_6_m3.z, ULP)
		);
	}
	void	MathTests::testVector3_dot() {
		Math::Vector3	v20(9, 2, 7);
		Math::Vector3	v21(4, 8, 10);
		float			dot = Math::Vector3::dot(v20, v21);
		TEST("Math::Vector3 dot(v1, v2)", isEqualf(dot, v.dot_122, ULP));
	}
#endif

#if TEST_MATH_ROTATION
	void	MathTests::testRotation_everything() {
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
	void	MathTests::testRotation() {

		Math::Rotation	rot1;
		TEST("Math::Rotation()", isEqualf(rot1.x, v.rotA_0_0_0_d.x, ULP) &&
								isEqualf(rot1.y, v.rotA_0_0_0_d.y, ULP) &&
								isEqualf(rot1.z, v.rotA_0_0_0_d.z, ULP) &&
								rot1.isDegree()
		);
		Math::Rotation	rot2(1, 2, 3);
		TEST("Math::Rotation(x, y, z)", isEqualf(rot2.x, v.rotB_1_2_3_d.x, ULP) &&
										isEqualf(rot2.y, v.rotB_1_2_3_d.y, ULP) &&
										isEqualf(rot2.z, v.rotB_1_2_3_d.z, ULP) &&
										rot2.isDegree()
		);
		Math::Rotation	rot3(1, 2, 3, ROT_DEG);
		TEST("Math::Rotation(x, y, z, ROT_DEG)", isEqualf(rot3.x, v.rotB_1_2_3_d.x, ULP) &&
												isEqualf(rot3.y, v.rotB_1_2_3_d.y, ULP) &&
												isEqualf(rot3.z, v.rotB_1_2_3_d.z, ULP) &&
												rot3.isDegree()
		);
		Math::Rotation	rot4(3.14159f, 0, 1.74533f, ROT_RAD);	// 180, 0, 100
		TEST("Math::Rotation(x, y, z, ROT_RAD)", isEqualf(rot4.x, v.rotC_r.x, ULP) &&
												isEqualf(rot4.y, v.rotC_r.y, ULP) &&
												isEqualf(rot4.z, v.rotC_r.z, ULP) &&
												rot4.isRadian()
		);
	}
	void	MathTests::testRotation_operatorEqual() {
		Math::Rotation	rot5 = v.rotC_r;	// 180, 0, 100
		TEST("Math::Rotation operator=", isEqualf(rot5.x, v.rotC_r.x, ULP) &&
										isEqualf(rot5.y, v.rotC_r.y, ULP) &&
										isEqualf(rot5.z, v.rotC_r.z, ULP) &&
										rot5.isRadian()
		);
	}
	void	MathTests::testRotation_setAsRad() {
		Math::Rotation	rot6 = v.rotD_180_0_100_d;
		rot6.setAsRad();
		TEST("Math::Rotation setAsRad()", isEqualf(rot6.x, v.rotC_r.x, ULP) &&
											isEqualf(rot6.y, v.rotC_r.y, ULP) &&
											isEqualf(rot6.z, v.rotC_r.z, ULP) &&
											rot6.isRadian()
		);
	}
	void	MathTests::testRotation_setAsDegree() {
		Math::Rotation	rot7 = v.rotC_r;	// 180, 0, 100
		rot7.setAsDegree();
		TEST("Math::Rotation setAsDegree()", isEqualf(rot7.x, v.rotD_180_0_100_d.x, ULP) &&
											isEqualf(rot7.y, v.rotD_180_0_100_d.y, ULP) &&
											isEqualf(rot7.z, v.rotD_180_0_100_d.z, ULP) &&
											rot7.isDegree()
		);
	}
	void	MathTests::testRotation_setUnit() {
		v.rotC_r.setUnit(ROT_DEG);
		TEST("Math::Rotation SetUnit(ROT_DEG)", v.rotC_r.isDegree());
		v.rotC_r.setUnit(ROT_RAD);
		TEST("Math::Rotation SetUnit(ROT_RAD)", v.rotC_r.isRadian());
	}
	void	MathTests::testRotation_toRadian() {
		Math::Rotation		rot8 = v.rotD_180_0_100_d.toRadian();
		TEST("Math::Rotation toRadian()", isEqualf(rot8.x, v.rotC_r.x, ULP) &&
											isEqualf(rot8.y, v.rotC_r.y, ULP) &&
											isEqualf(rot8.z, v.rotC_r.z, ULP) &&
											rot8.isRadian()
		);
	}
	void	MathTests::testRotation_toDegree() {
		Math::Rotation		rot9 = v.rotC_r.toDegree();	// 180 0 100 deg
		TEST("Math::Rotation toDegree()", isEqualf(rot9.x, v.rotD_180_0_100_d.x, ULP) &&
											isEqualf(rot9.y, v.rotD_180_0_100_d.y, ULP) &&
											isEqualf(rot9.z, v.rotD_180_0_100_d.z, ULP) &&
											rot9.isDegree()
		);
	}
	void	MathTests::testRotation_isRadian() {
		Math::Rotation	rot0;
		TEST("Math::Rotation isRadian()", !rot0.isRadian());

	}
	void	MathTests::testRotation_isDegree() {
		Math::Rotation	rot0;
		TEST("Math::Rotation isDegree()", rot0.isDegree());
	}
#endif

#if TEST_MATH_MATRIX4
	void	MathTests::testMatrix4_everything() {
		// prerequisite
		testMatrix4_identity();
		testMatrix4_reset();

		// constructors
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
	void	MathTests::testMatrix4() {
		FreeAccess<Math::Matrix4>	m3;	
		TEST("Math::Matrix4 Matrix4()", isEqualMat4(&m3, &(v.mat4_identity)));
	}
	void	MathTests::testMatrix4_operatorEqual() {
		FreeAccess<Math::Matrix4>	m4 = v.mat4_0_to_15_RM;
		FreeAccess<Math::Matrix4>	m5 = v.mat4_15_to_0_CM;
		TEST("Math::Matrix4 operator=", isEqualMat4(&m4, &(v.mat4_0_to_15_RM)));
		TEST("Math::Matrix4 operator=", isEqualMat4(&m5, &(v.mat4_15_to_0_CM)));
	}
	void	MathTests::testMatrix4_projectionMatrix() {
		FreeAccess<Math::Matrix4>	m6;
		float	fovRadian = 0;
		float	far = 0;
		float	near = 0;
		float	win_width = 0;
		float	win_height = 0;
		m6.projectionMatrix(fovRadian, far, near, win_width, win_height);
		NOT_TESTED("Math::Matrix4 projectionMatrix", " : ca peut dependre du rendu que l'on veut");
		/*
			see math.cpp
			la matrice peut changer selon qu'on veux etirer le rendu dans la fenetre, ou bien toujours garder un aspect ratio de 1/1.
			+ voir les diff avec https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix
		*/		
	}
	void	MathTests::testMatrix4_viewMatrix() {
		FreeAccess<Math::Matrix4>	m7;
		m7.viewMatrix(v.vecD_3_4_5, v.rotD_180_0_100_d);
		NOT_TESTED("Math::Matrix4 viewMatrix", " : a faire une fois que tout le reste est fait");
	}
	void	MathTests::testMatrix4_modelMatrix() {
		FreeAccess<Math::Matrix4>	m8;
		m8.modelMatrix(v.vecD_3_4_5, v.rotD_180_0_100_d, v.vecE_1_1_1);
		NOT_TESTED("Math::Matrix4 modelMatrix", " : a faire une fois que tout le reste est fait");
	}
	void	MathTests::testMatrix4_scaleMatrix() {
		FreeAccess<Math::Matrix4>	m9;
		m9.scaleMatrix(v.vecE_1_1_1);
		TEST("Math::Matrix4 scale(1, 1, 1)", isEqualMat4(&m9, &(v.mat4_scale_1_1_1_RM)));
		m9.scaleMatrix(v.vecB_4_5_6);
		TEST("Math::Matrix4 scale(4, 5, 6)", isEqualMat4(&m9, &(v.mat4_scale_4_5_6_RM)));
	}
	void	MathTests::testMatrix4_identity() {
		FreeAccess<Math::Matrix4>	m1;
		for (int i = 0; i < 15; i++) {
			m1.e[i] = i;
		}
		m1.identity();
		m1._order = ROW_MAJOR;
		TEST("Math::Matrix4 identity()", isEqualMat4(&m1, &(v.mat4_identity)));
	}
	void	MathTests::testMatrix4_reset() {
		FreeAccess<Math::Matrix4>	m2;
		for (int i = 0; i < 15; i++) {
			m2.e[i] = i;
		}
		m2._order = ROW_MAJOR;
		m2.reset();
		TEST("Math::Matrix4 reset()", isEqualMat4(&m2, &(v.mat4_clear)));
	}
	void	MathTests::testMatrix4_transpose() {
		FreeAccess<Math::Matrix4>	m10 = v.mat4_0_to_15_RM;
		m10.transpose();
		TEST("Math::Matrix4 transpose()", isEqualMat4(&m10, &(v.mat4_0_to_15_RM_transposed)));
	}
	void	MathTests::testMatrix4_printData() {
		NOT_TESTED("Math::Matrix4 printData()", " : not needed");
	}
	void	MathTests::testMatrix4_updatePosValue() {
		NOT_TESTED("Math::Matrix4 updatePosValue", " : a faire une fois que tout le reste est fait");
	}
	void	MathTests::testMatrix4_div() {
		FreeAccess<Math::Matrix4>	m13 = v.mat4_only8_RM;
		m13.div(4.0f);
		TEST("Math::Matrix4 div()", isEqualMat4(&m13, &(v.mat4_only2_RM)));
	}
	void	MathTests::testMatrix4_mult() {
		FreeAccess<Math::Matrix4>	m14 = v.mat4_only2_RM;
		m14.mult(4.0f);
		TEST("Math::Matrix4 mult()", isEqualMat4(&m14, &(v.mat4_only8_RM)));
	}
	void	MathTests::testMatrix4_getData() {
		float	*data = v.mat4_0_to_15_RM.getData();
		TEST("Math::Matrix4 getData()", data == &(v.mat4_0_to_15_RM.e[0]));
		
	}
	void	MathTests::testMatrix4_getOrder() {
		uint8_t		order = v.mat4_0_to_15_RM.getOrder();
		TEST("Math::Matrix4 getOrder()", order == v.mat4_0_to_15_RM._order);
	}
#endif