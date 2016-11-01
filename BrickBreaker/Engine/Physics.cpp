#include "Physics.h"

bool PhysicsObject::isCollidingWith(PhysicsObject& otherObj) {
	if (_type == CollisionType::Circle &&
		otherObj.getType() == CollisionType::Circle) {
		return collideCircle(otherObj);
	}

	return false;
}

bool PhysicsObject::collideCircle(PhysicsObject& otherObj) {
	_distanceSquared = *getCenter() - *otherObj.getCenter();
	_distanceSquared.x *= _distanceSquared.x;
	_distanceSquared.y *= _distanceSquared.y;

	_sumRadiiSquared = (_radius * _gameObject->getScale()) + (otherObj.getRadius() * otherObj.getGameObject->getScale());
	_sumRadiiSquared *= _sumRadiiSquared;

	if (_distanceSquared.x + _distanceSquared.y <= _sumRadiiSquared) {
		return true;
	}

	return false;
}

const VECTOR2* PhysicsObject::getCenter() {
	return _gameObject->getCenter();
}