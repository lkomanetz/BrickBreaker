#include "GameObject.h"

GameObject::GameObject() :
	Image() {

	//TODO(Logan) -> Add physics related initialization here.
	_health = 0;
	_active = true;
}

GameObject::GameObject(const GameObject& obj) :
	Image(obj) {
	
	_health = obj._health;
	p_input = obj.p_input;
}

GameObject& GameObject::operator=(const GameObject& obj) {
	Image::operator=(obj);
	_health = obj._health;
	p_input = obj.p_input;

	return *this;
}

void GameObject::update(float frameTime) {
	Image::update(frameTime);
}

bool GameObject::initialize(Game* pGame, int width, int height, int columnCount, TextureManager* pTextureManager) {
	p_input = pGame->getInput();
	return Image::initialize(pGame->getGraphics(), width, height, columnCount, pTextureManager);
}
