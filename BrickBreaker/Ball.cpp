#include "Ball.h"

Ball::Ball() {
	initialize();
}

Ball::Ball(Game* pGame, TextureManager* pTextureManager) {
	initialize();

	GameObject::initialize(
		pGame,
		_imageWidth,
		_imageHeight,
		_numberOfColumns,
		pTextureManager
	);
}

void Ball::initialize() {
	_imageWidth = 16;
	_imageHeight = 16;
	_numberOfColumns = 0;
}