#include "gamemanager.hpp"

GameManager::GameManager() {
	this->glfw = nullptr;
	this->cam = nullptr;
	this->objectList = nullptr;
	this->currentSelection = nullptr;
}

GameManager::~GameManager() {
}