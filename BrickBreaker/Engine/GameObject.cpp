#include "GameObject.h"

using namespace std;

GameObject::GameObject() :
	Image() {

	_health = 0;
	_active = true;
	_radius = 1.0;
	_edge.left = -1;
	_edge.top = -1;
	_edge.right = 1;
	_edge.bottom = 1;
	_mass = 1.0;
	_velocity.x = 0.0f;
	_velocity.y = 0.0f;
	_deltaVelocity.x = 0.0f;
	_deltaVelocity.y = 0.0f;
	_collisionType = CollisionType::None;
	_gravity = gameObjectNS::GRAVITY;
}

GameObject::GameObject(const GameObject& obj) :
	Image(obj) {
	
	_health = obj._health;
	p_input = obj.p_input;
	_active = obj._active;
	_radius = obj._radius;
	_edge = obj._edge;
	_mass = obj._mass;
	_velocity = obj._velocity;
	_deltaVelocity = obj._deltaVelocity;
	_collisionType = obj._collisionType;
	_gravity = obj._gravity;
}

GameObject& GameObject::operator=(const GameObject& obj) {
	Image::operator=(obj);

	_health = obj._health;
	p_input = obj.p_input;
	_active = obj._active;
	_radius = obj._radius;
	_edge = obj._edge;
	_mass = obj._mass;
	_velocity = obj._velocity;
	_deltaVelocity = obj._deltaVelocity;
	_collisionType = obj._collisionType;
	_gravity = obj._gravity;

	return *this;
}

void GameObject::update(float frameTime) {
	_velocity += _deltaVelocity;
	_deltaVelocity.x = 0;
	_deltaVelocity.y = 0;
	Image::update(frameTime);
}

void GameObject::performAi(float frameTime, GameObject& otherObj) {}

bool GameObject::initialize(Game* pGame, int width, int height, int columnCount, TextureManager* pTextureManager) {
	p_input = pGame->getInput();
	return Image::initialize(pGame->getGraphics(), width, height, columnCount, pTextureManager);
}

bool GameObject::isCollidingWith(GameObject& otherObj, VECTOR2& collisionVector) {
	if (!_active || !otherObj.isActive()) {
		return false;
	}

	if (_collisionType == CollisionType::Circle && otherObj.getCollisionType() == CollisionType::Circle) {
		return collideCircle(otherObj, collisionVector);
	}
	if (_collisionType == CollisionType::Box && otherObj.getCollisionType() == CollisionType::Box) {
		return collideBox(otherObj, collisionVector);
	}

	return false;
}

const VECTOR2* GameObject::getCenter() {
	_center = VECTOR2(getCenterX(), getCenterY());
	return &_center;
}

const VECTOR2* GameObject::getCorner(UINT corner) const {
	if (corner >= 4) {
		corner = 0;
	}

	return &_corners[corner];
}

bool GameObject::collideCircle(GameObject& otherObj, VECTOR2& collisionVector) {
	_distanceSquared = *getCenter() - *otherObj.getCenter();
	Graphics::Vector2Square(&_distanceSquared);

	_sumRadiiSquared = (_radius * getScale()) + (otherObj.getRadius() * otherObj.getScale());
	_sumRadiiSquared *= _sumRadiiSquared;

	if (_distanceSquared.x + _distanceSquared.y <= _sumRadiiSquared) {
		collisionVector = *otherObj.getCenter() - *getCenter();
		
		return true;
	}
	return true;
}

bool GameObject::collideBox(GameObject& otherObj, VECTOR2& collisionVector) {
	if ((getCenterX() + _edge.right * getScale() >= otherObj.getCenterX() + otherObj.getEdge().left * otherObj.getScale()) &&
		(getCenterX() + _edge.left * getScale() <= otherObj.getCenterX() + otherObj.getEdge().right * otherObj.getScale()) &&
		(getCenterY() + _edge.bottom * getScale() >= otherObj.getCenterY() + otherObj.getEdge().top * otherObj.getScale()) &&
		(getCenterY() + _edge.top * getScale() <= otherObj.getCenterY() + otherObj.getEdge().bottom * otherObj.getScale())) {

		collisionVector = *otherObj.getCenter() - *getCenter();
		return true;
	}

	return false;
}

void GameObject::addGravitationalForce(GameObject* otherObj, float frameTime) {
	if (!_active || !otherObj->isActive()) {
		return;
	}

	_radiusSquared = pow(otherObj->getCenterX() - this->getCenterX(), 2)
		+ pow(otherObj->getCenterY() - this->getCenterY(), 2);

	_force = _gravity * otherObj->getMass() * _mass / _radiusSquared;
	VECTOR2 gravityVelocity(otherObj->getCenterX() - this->getCenterX(), otherObj->getCenterY() - this->getCenterY());
	Graphics::Vector2Normalize(&gravityVelocity);
	gravityVelocity *= _force * frameTime;
	_velocity += gravityVelocity;
}

bool GameObject::isOutsideRectangle(RECT rect) {
	if (_spriteData.x + _spriteData.width * getScale() < rect.left ||
		_spriteData.x > rect.right ||
		_spriteData.y + _spriteData.height * getScale() < rect.top ||
		_spriteData.y > rect.bottom) {

		return true;
	}

	return false;
}
