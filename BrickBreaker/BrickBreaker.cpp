#include "BrickBreaker.h"

BrickBreaker::BrickBreaker() {
}

BrickBreaker::~BrickBreaker() {
	std::vector<GameObject*>::iterator iter;
	for (iter = _gameObjects.begin(); iter != _gameObjects.end(); iter++) {
		delete (*iter);
	}
}

void BrickBreaker::initialize(HWND hwnd) {
	Game::initialize(hwnd);

	GameObject* _nebula = new GameObject();
	GameObject* _planet = new GameObject();

	if (!_nebula->initialize(p_graphics, 0, 0, 0, NEBULA_IMAGE)) {
		throw GameError(GameErrorNS::FATAL_ERROR, "Error initializing nebula image.");
	}
	if (!_planet->initialize(p_graphics, 0, 0, 0, PLANET_IMAGE)) {
		throw GameError(GameErrorNS::FATAL_ERROR, "Error initializing planet image.");
	}

	_planet->setX(GAME_WIDTH * 0.5f - _planet->getWidth() * 0.5f);
	_planet->setY(GAME_HEIGHT * 0.5f - _planet->getHeight() * 0.5f);

	_gameObjects.push_back(_nebula);
	_gameObjects.push_back(_planet);
	return;
}

void BrickBreaker::update() {
	return;
}

void BrickBreaker::performAi() {
	return;
}

void BrickBreaker::render() {
	p_graphics->spriteBegin();

	std::vector<GameObject*>::iterator iter;
	for (iter = _gameObjects.begin(); iter != _gameObjects.end(); iter++) {
		(*iter)->draw();
	}

	p_graphics->spriteEnd();

	return;
}

void BrickBreaker::calculateCollisions() {
	return;
}

void BrickBreaker::releaseAll() {
	UINT arraySize = sizeof(_gameObjects) / sizeof(GameObject*);
	for (UINT i = 0; i < arraySize; i++) {
		_gameObjects[i]->getTextureManager()->onLostDevice();
	}
	Game::releaseAll();

	return;
}

void BrickBreaker::resetAll() {
	UINT arraySize = sizeof(_gameObjects) / sizeof(GameObject*);
	for (UINT i = 0; i < arraySize; i++) {
		_gameObjects[i]->getTextureManager()->onResetDevice();
	}
	Game::resetAll();

	return;
}