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
	_imageWidth = 128;
	_imageHeight = 28;
	_numberOfColumns = 0;
}