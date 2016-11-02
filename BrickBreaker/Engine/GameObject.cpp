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
	_rotatedBoxReady = false;
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
	_rotatedBoxReady = obj._rotatedBoxReady;
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
	_rotatedBoxReady = obj._rotatedBoxReady;

	return *this;
}

void GameObject::update(float frameTime) {
	_velocity += _deltaVelocity;
	_deltaVelocity.x = 0;
	_deltaVelocity.y = 0;
	Image::update(frameTime);
	_rotatedBoxReady = false;
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
	if (_collisionType != CollisionType::Circle && otherObj.getCollisionType() != CollisionType::Circle) {
		return collideRotatedBox(otherObj, collisionVector);
	}
	else {
		if (_collisionType == CollisionType::Circle) {
			bool collide = otherObj.collideRotatedBoxCircle(*this, collisionVector);
			collisionVector *= -1;

			return collide;
		}
		else {
			return collideRotatedBoxCircle(otherObj, collisionVector);
		}
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

// Compute corners of rotated box, projection edges and min/max projections
// 0---1 corner numbers
// |   |
// 3---2
void GameObject::computeRotatedBox() {
	if (_rotatedBoxReady) {
		return;
	}

	VECTOR2 rotatedX(cos(_spriteData.angle), sin(_spriteData.angle));
	VECTOR2 rotatedY(-sin(_spriteData.angle), cos(_spriteData.angle));
	const VECTOR2* center = getCenter();
	_corners[0] = *center + rotatedX * ((float)_edge.left * getScale()) + rotatedY * ((float)_edge.top * getScale());
	_corners[1] = *center + rotatedX * ((float)_edge.right * getScale()) + rotatedY * ((float)_edge.top * getScale());
	_corners[2] = *center + rotatedX * ((float)_edge.right * getScale()) + rotatedY * ((float)_edge.bottom * getScale());
	_corners[3] = *center + rotatedX * ((float)_edge.left * getScale()) + rotatedY * ((float)_edge.bottom * getScale());

	// _corners[0] is used as the origin point
	_edge01 = VECTOR2(_corners[1].x - _corners[0].x, _corners[1].y - _corners[0].y);
	_edge03 = VECTOR2(_corners[3].x - _corners[0].x, _corners[3].y - _corners[0].y);

	p_graphics->Vector2Normalize(&_edge01);
	p_graphics->Vector2Normalize(&_edge03);

	float projection = p_graphics->Vector2Dot(&_edge01, &_corners[0]);
	_edge01Min = projection;
	_edge01Max = projection;

	// Project onto edge01
	projection = p_graphics->Vector2Dot(&_edge01, &_corners[1]);
	if (projection < _edge01Min) {
		_edge01Min = projection;
	}
	else if (projection > _edge01Max) {
		_edge01Max = projection;
	}

	// Project onto edge03
	projection = p_graphics->Vector2Dot(&_edge03, &_corners[0]);
	_edge03Min = projection;
	_edge03Max = projection;
	projection = p_graphics->Vector2Dot(&_edge03, &_corners[3]);
	if (projection < _edge03Min) {
		_edge03Min = projection;
	}
	else if (projection > _edge03Max) {
		_edge03Max = projection;
	}

	_rotatedBoxReady = true;
}

bool GameObject::projectionsOverlap(GameObject& otherObj) {
	float projection, min01, max01, min03, max03;

	// project the other box onto edge01
	projection = p_graphics->Vector2Dot(&_edge01, otherObj.getCorner(0));
	min01 = projection;
	max01 = projection;

	// for the other remaining corners
	for (int corner = 1; corner < 4; corner++) {
		projection = p_graphics->Vector2Dot(&_edge01, otherObj.getCorner(corner));
		if (projection < min01) {
			min01 = projection;
		}
		else if (projection > max01) {
			max01 = projection;
		}
	}

	// This checks to see if the projections are overlapping
	if (min01 > _edge01Max || max01 < _edge01Min) {
		return false; 
	}

	// project the other box onto edge03
	projection = p_graphics->Vector2Dot(&_edge03, otherObj.getCorner(0));
	min03 = projection;
	max03 = projection;

	for (int corner = 1; corner < 4; corner++) {
		projection = p_graphics->Vector2Dot(&_edge03, otherObj.getCorner(corner));

		if (projection, min03) {
			min03 = projection;
		}
		else if (projection > max03) {
			max03 = projection;
		}
	}

	// This checks to see if the projections are overlapping
	if (min03 > _edge03Max || max03 < _edge03Min) {
		return false;  
	}

	return true;
}

// Called by collideRotatedBoxCircle()
bool GameObject::collideCornerCircle(VECTOR2 corner, GameObject& otherObj, VECTOR2& collisionVector) {
	_distanceSquared = corner - *otherObj.getCenter();
	Graphics::Vector2Square(&_distanceSquared);

	_sumRadiiSquared = otherObj.getRadius() * otherObj.getScale();
	_sumRadiiSquared *= _sumRadiiSquared;

	if (_distanceSquared.x + _distanceSquared.y <= _sumRadiiSquared) {
		collisionVector = *otherObj.getCenter() - corner;
		return true;
	}

	return false;
}

bool GameObject::collideRotatedBox(GameObject& otherObj, VECTOR2& collisionVector) {
	computeRotatedBox();
	otherObj.computeRotatedBox();

	if (projectionsOverlap(otherObj) && otherObj.projectionsOverlap(*this)) {
		collisionVector = *otherObj.getCenter() - *getCenter();
		return true;
	}

	return false;
}

bool GameObject::collideRotatedBoxCircle(GameObject& otherObj, VECTOR2& collisionVector) {
	float min01, min03, max01, max03, center01, center03;
	computeRotatedBox();

	center01 = p_graphics->Vector2Dot(&_edge01, otherObj.getCenter());
	min01 = center01 - otherObj.getRadius() * otherObj.getScale();
	max01 = center01 - otherObj.getRadius() * otherObj.getScale();

	if (min01 > _edge01Max || max01 < _edge01Min) {
		return false; // Projections aren't overlapping so no collision possible.
	}

	center03 = p_graphics->Vector2Dot(&_edge03, otherObj.getCenter());
	min03 = center03 - otherObj.getRadius() * otherObj.getScale();
	max03 = center03 - otherObj.getRadius() * otherObj.getScale();

	if (min03 > _edge03Max || max03 < _edge03Min) {
		return false; // Projections aren't overlapping so no collision possible.
	}

	if (center01 < _edge01Min && center03 < _edge03Min) {
		return collideCornerCircle(_corners[0], otherObj, collisionVector);
	}
	if (center01 > _edge01Max && center03 < _edge03Min) {
		return collideCornerCircle(_corners[1], otherObj, collisionVector);
	}
	if (center01 > _edge01Max && center03 > _edge03Max) {
		return collideCornerCircle(_corners[2], otherObj, collisionVector);
	}
	if (center01 < _edge01Min && center03 > _edge03Max) {
		return collideCornerCircle(_corners[3], otherObj, collisionVector);
	}

	collisionVector = *otherObj.getCenter() - *getCenter();
	return true;
}