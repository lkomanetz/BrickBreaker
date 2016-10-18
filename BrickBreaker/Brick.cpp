#include "Brick.h"

Brick::Brick() {}

Brick::Brick(Graphics* pGraphics) {
	this->initialize(pGraphics, 0, 0, 0, _imageLocation);
}

Brick::Brick(const Brick& obj) : GameObject(obj) {
	_imageLocation = obj._imageLocation;
	_type = obj._type;
}

Brick& Brick::operator=(const Brick& rightObj) {
	GameObject::operator=(rightObj);
	_imageLocation = rightObj._imageLocation;
	_type = rightObj._type;
	return *this;
}

Brick::~Brick() {
}