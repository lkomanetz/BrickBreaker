#include "Brick.h"

Brick::Brick() {}

Brick::Brick(Graphics* pGraphics, TextureManager* textureManager) {
	this->initialize(pGraphics,
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