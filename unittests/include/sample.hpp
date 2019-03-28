#pragma once

#include "unittests.hpp"
#include "properties.hpp"

class PropertiesTests;

template <typename T>
class FreeAccess : public T {
public:
	FreeAccess() : T() {} //if the class T doesnt have default constructor? boom?
	friend class PropertiesTests;
};


class PropertiesTests {
public:
	void	setUp();
	void	tearDown();
	void	testEverything();

};