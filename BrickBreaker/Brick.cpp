#include "Brick.h"

Brick::Brick() {
	initialize();
}

Brick::Brick(Graphics* pGraphics, TextureManager* textureManager) {
	initialize();
	GameObject::initialize(
		pGraphics,
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
	_imageWidth = 59;
	_imageHeight = 18;
	_numberOfColumns = 3;
}