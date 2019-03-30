#include "object_tests.hpp"


void	UnitTests::ObjectTests::setUp() {
	objectA._parent = NULL;
	objectB._parent = &objectA;
	objectB._worldMatrixChanged = false;
	ppA_pos_1_2_3._pos.x = 1;
	ppA_pos_1_2_3._pos.y = 2;
	ppA_pos_1_2_3._pos.z = 3;
}
void	UnitTests::ObjectTests::tearDown() {
}
void	UnitTests::ObjectTests::testEverything() const {
	std::cout << "object tests" << std::endl;
	testObject();
	testObject_operatorEqual();
	testObject_update();
	testObject_render();
	testObject_setParent();
	testObject_getId();
	testObject_getLocalProperties();
	testObject_getWorldMatrix();
	testObject_getParent();
}

void	UnitTests::ObjectTests::testObject() const {
	unsigned int	oldInstanceAmount = Object::_instanceAmount;
	unsigned int	oldInstanceId = Object::_instanceId;
	Object*			obj1 = new Object();
	TEST("Object()", obj1->_worldMatrixChanged == true);
	TEST("Object()", obj1->_id == oldInstanceId);
	TEST("Object()", obj1->_parent == NULL);
	TEST("Object()", Object::_instanceAmount == oldInstanceAmount + 1);
	TEST("Object()", Object::_instanceId == oldInstanceId + 1);
	delete obj1;
	TEST("Object() delete", Object::_instanceAmount == oldInstanceAmount);

	Object	obj2(ppA_pos_1_2_3);
	TEST("Object(local_properties)", UnitTests::MathTests::isEqualVector3(&obj2.local._pos, &ppA_pos_1_2_3._pos));
	NOT_TESTED("Object(local_properties)", " : need human touch (check if Properties cpy onstructor is called)", UnitTests::test_human);

}
void	UnitTests::ObjectTests::testObject_operatorEqual() const {
	unsigned int	oldInstanceAmount = Object::_instanceAmount;
	unsigned int	oldInstanceId = Object::_instanceId;
	Object			obj3 = objectB;
	TEST("Object operator=", obj3._worldMatrixChanged == true);
	TEST("Object operator=", obj3._id != objectB._id);
	TEST("Object operator=", obj3._id == oldInstanceId);
	TEST("Object operator=", obj3._parent == &objectA);
	TEST("Object operator=", UnitTests::MathTests::isEqualMat4(&obj3._worldMatrix, &objectB._worldMatrix));
	TEST("Object operator=", Object::_instanceAmount == oldInstanceAmount + 1);
	TEST("Object operator=", Object::_instanceId == oldInstanceId + 1);
	NOT_TESTED("Object operator=", " : need human touch (check if Properties::operator= is called)", UnitTests::test_human);
}
void	UnitTests::ObjectTests::testObject_update() const {
	Object			obj4_parent;
	Object			obj5_child;
	Math::Matrix4	mat4_obj4 = obj4_parent.local._matrix;
	Math::Matrix4	mat4_obj5 = obj5_child.local._matrix;

	obj4_parent.local._pos.x = 4;
	obj4_parent.local._pos.x = 5;
	obj4_parent.local._pos.x = 6;
	obj4_parent.local._matrixUpdated = false;
	obj4_parent.local._matrixChanged = false;
	obj4_parent._worldMatrixChanged = false;
	obj5_child._parent = &obj4_parent;
	obj5_child.local._matrixUpdated = false;
	obj5_child.local._matrixChanged = false;
	obj5_child._worldMatrixChanged = false;
	obj5_child.update();

	TEST("Object update", obj4_parent.local._matrixUpdated == true);
	TEST("Object update", obj4_parent.local._matrixChanged == true);
	TEST("Object update", obj4_parent._worldMatrixChanged == true);
	TEST("Object update", !UnitTests::MathTests::isEqualMat4(&obj4_parent.local._matrix, &mat4_obj4));
	TEST("Object update", obj5_child.local._matrixUpdated == true);
	TEST("Object update", obj5_child.local._matrixChanged == true);
	TEST("Object update", obj5_child._worldMatrixChanged == true);
	TEST("Object update", !UnitTests::MathTests::isEqualMat4(&obj5_child.local._matrix, &mat4_obj5));
	// NOT_TESTED("Object operator=", " : need human touch (check if parent's update is called)", UnitTests::test_human); // above tests should confirm that
}
void	UnitTests::ObjectTests::testObject_render() const {
	Object			obj6;
	Math::Matrix4	mat4_obj6 = obj6.local._matrix;
	Math::Matrix4	fake_PVmat4;

	obj6.local._pos.x = 4;
	obj6.local._pos.x = 5;
	obj6.local._pos.x = 6;
	obj6.local._matrixUpdated = false;
	obj6.local._matrixChanged = false;
	obj6._worldMatrixChanged = false;

	obj6.render(fake_PVmat4);
	TEST("Object render", obj6.local._matrixUpdated == true);
	TEST("Object render", obj6.local._matrixChanged == true);
	TEST("Object render", obj6._worldMatrixChanged == true);
	TEST("Object render", !UnitTests::MathTests::isEqualMat4(&obj6.local._matrix, &mat4_obj6));
	// NOT_TESTED("Object render", " : need human touch (check if the object is rendered)", UnitTests::test_human);	// not needed : it's an empty object
}
void	UnitTests::ObjectTests::testObject_setParent() const {
	Object		obj7;
	Object		obj8;
	obj7.setParent(&obj8);
	TEST("Object setParent", obj7._parent == &obj8);
}
void	UnitTests::ObjectTests::testObject_getId() const {
	unsigned int	id = objectA.getId();
	TEST("Object getId", id == objectA._id);
}
void	UnitTests::ObjectTests::testObject_getLocalProperties() const {
	Object		obj9(ppA_pos_1_2_3);
	Properties	pp = obj9.getLocalProperties();
	TEST("Object getLocalProperties", UnitTests::MathTests::isEqualVector3(&pp._pos, &ppA_pos_1_2_3._pos));
	NOT_TESTED("Object getLocalProperties", " : need human touch (check if Properties cpy onstructor is called)", UnitTests::test_human);
}
void	UnitTests::ObjectTests::testObject_getWorldMatrix() const {
	Math::Matrix4&	mat4 = objectA.getWorldMatrix();
	TEST("Object getWorldMatrix", &mat4 == &objectA._worldMatrix);
}
void	UnitTests::ObjectTests::testObject_getParent() const {
	Object	*parent = objectB.getParent();
	TEST("Object getParent", parent == objectB._parent);
	TEST("Object getParent", parent == &objectA);
}
