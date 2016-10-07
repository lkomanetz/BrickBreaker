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

	GameObject* _nebula = new GameObject(p_graphics, 0, 0, 0, NEBULA_IMAGE);
	GameObject* _planet = new GameObject(p_graphics, 0, 0, 0, PLANET_IMAGE);
	GameObject* _brick = new GameObject(p_graphics, 0, 0, 0, BRICK_IMAGE);

	_planet->setX(GAME_WIDTH * 0.5f - _planet->getWidth() * 0.5f);
	_planet->setY(GAME_HEIGHT * 0.5f - _planet->getHeight() * 0.5f);
	_brick->setX(((GAME_WIDTH * 0.5F) - 1.0F) - _brick->getWidth() * 0.5f);
	_brick->setY(((GAME_HEIGHT * 0.5F) - 1.0F) - _brick->getHeight() * 0.5f);

	_gameObjects.push_back(_nebula);
	_gameObjects.push_back(_planet);
	_gameObjects.push_back(_brick);
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