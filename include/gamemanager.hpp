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

	Glfw*				glfw = nullptr;
	Cam*				cam = nullptr;
	std::list<Object*>*	objectList = nullptr;
	Object*				currentSelection = nullptr;
private:
};