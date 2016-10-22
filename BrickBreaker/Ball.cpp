#include "Ball.h"

Ball::Ball() {
	initialize();
}

Ball::Ball(Graphics* pGraphics, TextureManager* pTextureManager) {
	initialize();

	GameObject::initialize(
		pGraphics,
		_imageWidth,
		_imageHeight,
		_numberOfColumns,
		pTextureManager
	);
}

void Ball::initialize() {
	_imageWidth = 16;
	_imageHeight = 16;
	_numberOfColumns = 3;
}