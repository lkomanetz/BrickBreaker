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
	_spriteData.width = _imageWidth;
	_spriteData.height = _imageHeight;
	_spriteData.x = (GAME_WIDTH * 0.5f) - (this->getWidth() * 0.5f);
	_spriteData.y = (GAME_HEIGHT - this->getHeight() - 50.0f);
	_spriteData.rectangle.bottom = _imageHeight;
	_spriteData.rectangle.right = _imageWidth;
	_frameDelay = 0.0f;
	_startFrame = 0;
	_endFrame = 0;
	_currentFrame = _startFrame;
	_spriteData.scale = 1.25f;
}

void Ball::update(float frameTime) {
	GameObject::update(frameTime);
}