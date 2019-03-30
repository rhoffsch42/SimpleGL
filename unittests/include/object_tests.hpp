#pragma once

#include "unittests.hpp"
#include "object.hpp"

class UnitTests::ObjectTests {
public:
	void	setUp();
	void	tearDown();
	void	testEverything() const;

	void	testObject() const;
	void	testObject_operatorEqual() const;
	void	testObject_update() const;
	void	testObject_render() const;
	void	testObject_setParent() const;
	void	testObject_getId() const;
	void	testObject_getLocalProperties() const;
	void	testObject_getWorldMatrix() const;
	void	testObject_getParent() const;
	// void	testObject_() const;
private:
	Object		objectA;
	Object		objectB;
	Properties	ppA_pos_1_2_3;
};