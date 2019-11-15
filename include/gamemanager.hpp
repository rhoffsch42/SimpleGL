#pragma once

#include "glfw.hpp"
#include "cam.hpp"
#include "object.hpp"

class Glfw;
class Cam;

class GameManager {
public:
	GameManager();
	virtual ~GameManager();

	Glfw *			glfw;
	Cam *			cam;
	list<Object*> *	objectList;
	Object *		currentSelection;
private:
};