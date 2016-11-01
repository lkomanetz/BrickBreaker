#include "GameObject.h"

using namespace std;

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

bool GameObject::isCollidingWith(GameObject& otherObj) {
	if (!_active || !otherObj.isActive()) {
		return false;
	}

	auto currentPhysicsObj = _physicsObjects.begin();
	auto endPhysicsObj = _physicsObjects.end();
	while (currentPhysicsObj != endPhysicsObj) {
		auto otherCurrentPhysicsObj = otherObj.getPhysicsObjects().begin();
		auto otherEndPhysicsObj = otherObj.getPhysicsObjects().end();
		while (otherCurrentPhysicsObj != otherEndPhysicsObj) {
			if (currentPhysicsObj->isCollidingWith(*otherCurrentPhysicsObj)) {
				return true;
			}
			++otherCurrentPhysicsObj;
		}

		++currentPhysicsObj;
	}

	return false;
}

const VECTOR2* GameObject::getCenter() {
	_center = VECTOR2(getCenterX(), getCenterY());
	return &_center;
}