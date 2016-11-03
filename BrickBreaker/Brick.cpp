#include "Brick.h"

Brick::Brick() {
	initialize();
}

Brick::Brick(Game* pGame, TextureManager* textureManager) {
	initialize();
	GameObject::initialize(
		pGame,
		_imageWidth,
		_imageHeight,
		_numberOfColumns,
		textureManager
	);
}

Brick::Brick(const Brick& obj) : GameObject(obj) {
	_type = obj._type;
}

Brick& Brick::operator=(const Brick& rightObj) {
	GameObject::operator=(rightObj);
	_type = rightObj._type;
	return *this;
}

Brick::~Brick() {
}

void Brick::initialize() {
	_imageWidth = brickNS::WIDTH;
	_imageHeight = brickNS::HEIGHT;
	_numberOfColumns = 3;
	_spriteData.width = brickNS::WIDTH;
	_spriteData.height = brickNS::HEIGHT;
	_spriteData.rectangle.bottom = brickNS::HEIGHT;
	_spriteData.rectangle.right = brickNS::WIDTH;
	_frameDelay = 0.0f;
	_startFrame = 0;
	_endFrame = 0;
	_currentFrame = _startFrame;
	_edge.top = -brickNS::HEIGHT / 2;
	_edge.bottom = brickNS::HEIGHT / 2;
	_edge.left = -brickNS::WIDTH / 2;
	_edge.right = brickNS::WIDTH / 2;
	_collisionType = CollisionType::RotatedBox;
}