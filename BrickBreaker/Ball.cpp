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
	_spriteData.scale = 1.25f;
	_spriteData.width = _imageWidth;
	_spriteData.height = _imageHeight;
	_spriteData.x = (GAME_WIDTH * 0.5f) - (this->getWidth() * 0.5f);
	_spriteData.y = (GAME_HEIGHT - this->getHeight() - 100.0f);
	_spriteData.rectangle.bottom = _imageHeight;
	_spriteData.rectangle.right = _imageWidth;
	_frameDelay = 0.0f;
	_startFrame = 0;
	_endFrame = 0;
	_currentFrame = _startFrame;
	_collisionType = CollisionType::Circle;
	_radius = _imageWidth / 2 * getScale();
	_mass = 0.0f;
}

void Ball::update(float frameTime) {
	GameObject::update(frameTime);
	_spriteData.x += frameTime * _velocity.x;
	_spriteData.y += frameTime * _velocity.y;

	if (_spriteData.x > GAME_WIDTH - _imageWidth * _spriteData.scale) {
		_spriteData.x = GAME_WIDTH - _imageWidth * _spriteData.scale;
		_velocity.x = -_velocity.x;
	}
	else if (_spriteData.x < 0) {
		_spriteData.x = 0;
		_velocity.x = -_velocity.x;
	}

	if (_spriteData.y > GAME_HEIGHT - _imageHeight * getScale()) {
		_spriteData.y = GAME_HEIGHT - _imageHeight * getScale();
		_velocity.y = -_velocity.y;
	}
	else if (_spriteData.y < 0) {
		_spriteData.y = 0;
		_velocity.y = -_velocity.y;
	}
}