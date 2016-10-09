#include "BrickBreaker.h"
#include <exception>

using namespace std;

BrickBreaker::BrickBreaker() {
	_objects = new Brick**[8];
	for (UINT i = 0; i < 8; i++) {
		_objects[i] = new Brick*[8];
	}
}

BrickBreaker::~BrickBreaker() {
	std::vector<GameObject*>::iterator iter;
	for (iter = _gameObjects.begin(); iter != _gameObjects.end(); iter++) {
		delete (*iter);
	}

	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			safeDelete(_objects[i][j]);
		}
		safeDeleteArray(_objects[i]);
	}
}

void BrickBreaker::initialize(HWND hwnd) {
	Game::initialize(hwnd);

	GameObject* _nebula = new GameObject(p_graphics, 0, 0, 0, NEBULA_IMAGE);
	GameObject* _planet = new GameObject(p_graphics, 0, 0, 0, PLANET_IMAGE);

	_planet->setX(GAME_WIDTH * 0.5f - _planet->getWidth() * 0.5f);
	_planet->setY(GAME_HEIGHT * 0.5f - _planet->getHeight() * 0.5f);

	_gameObjects.push_back(_nebula);
	_gameObjects.push_back(_planet);

	
	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			_objects[i][j] = new Brick(p_graphics);
			if ((j % 2) == 0) {
				_objects[i][j]->setBrickType(BrickType::NoBrick);
			}
			else {
				_objects[i][j]->setBrickType(BrickType::Normal);
			}
		}
	}
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

	float startX = 20.0f;
	float brickWidth = _objects[0][0]->getWidth();
	float brickHeight = _objects[0][0]->getHeight();
	float brickX = startX;
	float brickY = 0.0f;
	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			if (_objects[i][j]->getBrickType() != BrickType::NoBrick) {
				_objects[i][j]->setX(brickX);
				_objects[i][j]->setY(brickY);
				_objects[i][j]->draw();
			}
			brickX += brickWidth;
		}
		brickY += brickHeight;
		brickX = startX;
	}

	p_graphics->spriteEnd();

	return;
}

void BrickBreaker::calculateCollisions() {
	return;
}

void BrickBreaker::releaseAll() {
	vector<GameObject*>::iterator iter;
	for (iter = _gameObjects.begin(); iter != _gameObjects.end(); iter++) {
		(*iter)->getTextureManager()->onLostDevice();
	}
	Game::releaseAll();

	return;
}

void BrickBreaker::resetAll() {
	vector<GameObject*>::iterator iter;
	for (iter = _gameObjects.begin(); iter != _gameObjects.end(); iter++) {
		(*iter)->getTextureManager()->onResetDevice();
	}
	Game::resetAll();

	return;
}