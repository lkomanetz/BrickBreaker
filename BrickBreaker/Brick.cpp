#include "Brick.h"

Brick::Brick() {}

Brick::Brick(Graphics* pGraphics) {
	this->initialize(pGraphics, 0, 0, 0, _imageLocation);
}

Brick::~Brick() {
}