#include "Paddle.h"

Paddle::Paddle() {
	initialize();
}

Paddle::Paddle(Graphics* pGraphics, TextureManager* pTextureManager) {
	initialize();
	GameObject::initialize(
		pGraphics,
		_imageWidth,
		_imageHeight,
		_numberOfColumns,
		pTextureManager
	);
}

void Paddle::initialize() {
	_imageWidth = 34;
	_imageHeight = 8;
	_numberOfColumns = 3;
}