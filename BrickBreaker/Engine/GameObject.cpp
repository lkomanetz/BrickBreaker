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
	_pixelsColliding = 0;
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
	_pixelsColliding = obj._pixelsColliding;
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
	_pixelsColliding = obj._pixelsColliding;

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
			return this->collideRotatedBoxCircle(otherObj, collisionVector);
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

bool GameObject::projectionsOverlap(GameObject& otherObj, VECTOR2& collisionVector) {
	float projection, min01, max01, min03, max03, minOverlap, minOverlap2;

	projection = p_graphics->Vector2Dot(&_edge01, otherObj.getCorner(0));
	min01 = projection;
	max01 = projection;
	for (int corner = 1; corner < 4; corner++) {
		projection = p_graphics->Vector2Dot(&_edge01, otherObj.getCorner(corner));
		if (projection < min01) {
			min01 = projection;
		}
		else if (projection > max01) {
			max01 = projection;
		}
	}

	if (min01 > _edge01Max || max01 < _edge01Min) {
		return false;
	}

	projection = p_graphics->Vector2Dot(&_edge03, otherObj.getCorner(0));
	min03 = projection;
	max03 = projection;
	for (int corner = 1; corner < 4; corner++) {
		projection = p_graphics->Vector2Dot(&_edge03, otherObj.getCorner(corner));
		if (projection < min03) {
			min03 = projection;
		}
		else if (projection > max03) {
			max03 = projection;
		}
	}

	if (min03 > _edge03Max || max03 < _edge03Min) {
		return false;
	}

	if (min01 < _edge01Min) {
		minOverlap = max01 - _edge01Min;
		collisionVector = _corners[1] - _corners[0];
		_collisionCenter = _corners[1];
		otherObj.setCollisionCenter(_corners[0]);
	}
	else {
		minOverlap = _edge01Max - min01;
		collisionVector = _corners[0] - _corners[1];
		_collisionCenter = _corners[1];
		otherObj.setCollisionCenter(_corners[0]);
	}

	if (min03 < _edge03Min) {
		minOverlap2 = max03 - _edge03Min;
		if (minOverlap2 < minOverlap) {
			collisionVector = _corners[3] - _corners[0];
			_collisionCenter = _corners[0];
			otherObj.setCollisionCenter(_corners[3]);
		}
	}
	else {
		minOverlap2 = _edge03Max - min03;
		if (minOverlap2 < minOverlap) {
			collisionVector = _corners[0] - _corners[3];
			_collisionCenter = _corners[3];
			otherObj.setCollisionCenter(_corners[0]);
		}
	}

	return true;
}

// Compute corners of rotated box, projection edges and min/max projections
// 0---1  corner numbers
// |   |
// 3---2
void GameObject::computeRotatedBox() {
	if (_rotatedBoxReady) {
		return;
	}

	float projection;
	VECTOR2 rotatedX(cos(_spriteData.angle), sin(_spriteData.angle));
	VECTOR2 rotatedY(-sin(_spriteData.angle), cos(_spriteData.angle));
	const VECTOR2* pCenter = getCenter();

	_corners[0] = *pCenter + rotatedX * ((float)_edge.left*getScale()) +
		rotatedY * ((float)_edge.top*getScale());
	_corners[1] = *pCenter + rotatedX * ((float)_edge.right*getScale()) +
		rotatedY * ((float)_edge.top*getScale());
	_corners[2] = *pCenter + rotatedX * ((float)_edge.right*getScale()) +
		rotatedY * ((float)_edge.bottom*getScale());
	_corners[3] = *pCenter + rotatedX * ((float)_edge.left*getScale()) +
		rotatedY * ((float)_edge.bottom*getScale());

	_edge01 = VECTOR2(_corners[1].x - _corners[0].x, _corners[1].y - _corners[0].y);
	_edge03 = VECTOR2(_corners[3].x - _corners[0].x, _corners[3].y - _corners[0].y);

	p_graphics->Vector2Normalize(&_edge01);
	p_graphics->Vector2Normalize(&_edge03);

	projection = p_graphics->Vector2Dot(&_edge01, &_corners[0]);
	_edge01Min = projection;
	_edge01Max = projection;

	projection = p_graphics->Vector2Dot(&_edge01, &_corners[1]);
	if (projection < _edge01Min) {
		_edge01Min = projection;
	}
	else if (projection > _edge01Max) {
		_edge01Max = projection;
	}

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

bool GameObject::collideRotatedBox(GameObject& otherObj, VECTOR2& collisionVector) {
	this->computeRotatedBox();
	otherObj.computeRotatedBox();

	return (this->projectionsOverlap(otherObj, collisionVector) && otherObj.projectionsOverlap(*this, collisionVector));
}

bool GameObject::collideRotatedBoxCircle(GameObject& otherObj, VECTOR2& collisionVector) {
	float min01, min03, max01, max03, center01, center03, minOverlap, minOverlap2;
	this->computeRotatedBox();

	center01 = p_graphics->Vector2Dot(&_edge01, otherObj.getCenter());
	min01 = center01 - otherObj.getRadius() * otherObj.getScale();
	max01 = center01 + otherObj.getRadius() * otherObj.getScale();

	if (min01 > _edge01Max || max01 < _edge01Min) {
		return false;
	}

	center03 = p_graphics->Vector2Dot(&_edge03, otherObj.getCenter());
	min03 = center03 - otherObj.getRadius() * otherObj.getScale();
	max03 = center03 + otherObj.getRadius() * otherObj.getScale();

	if (min03 > _edge03Max || max03 < _edge03Min) {
		return false;
	}

	// circle projection overlaps box projection
	// check to see if circle is in voronoi region of collision box
	if (center01 < _edge01Min && center03 < _edge03Min)    // if circle in Voronoi0
		return collideCornerCircle(_corners[0], otherObj, collisionVector);
	if (center01 > _edge01Max && center03 < _edge03Min)    // if circle in Voronoi1
		return collideCornerCircle(_corners[1], otherObj, collisionVector);
	if (center01 > _edge01Max && center03 > _edge03Max)    // if circle in Voronoi2
		return collideCornerCircle(_corners[2], otherObj, collisionVector);
	if (center01 < _edge01Min && center03 > _edge03Max)    // if circle in Voronoi3
		return collideCornerCircle(_corners[3], otherObj, collisionVector);

	if (min01 < _edge01Min) {
		minOverlap = max01 - _edge01Min;
		collisionVector = _corners[1] - _corners[0];
		_collisionCenter = _corners[0];
		otherObj.setCollisionCenter(_corners[1]);
	}
	else {
		minOverlap = _edge01Max - min01;
		collisionVector = _corners[0] - _corners[1];
		_collisionCenter = _corners[1];
		otherObj.setCollisionCenter(_corners[0]);
	}

	if (min03 < _edge03Min) {
		minOverlap2 = max03 - _edge03Min;
		if (minOverlap2 < minOverlap) {
			collisionVector = _corners[3] - _corners[0];
			_collisionCenter = _corners[0];
			otherObj.setCollisionCenter(_corners[3]);
		}
	}
	else {
		minOverlap2 = _edge03Max - min03;
		if (minOverlap2 < minOverlap) {
			collisionVector = _corners[0] - _corners[3];
			_collisionCenter = _corners[3];
			otherObj.setCollisionCenter(_corners[0]);
		}
	}

	return true;
}

bool GameObject::collideCornerCircle(VECTOR2 corner, GameObject& otherObj, VECTOR2& collisionVector) {
	_distanceSquared = corner - *otherObj.getCenter();
	Graphics::Vector2Square(&_distanceSquared);
	_sumRadiiSquared = otherObj.getRadius() * otherObj.getScale();
	_sumRadiiSquared *= _sumRadiiSquared;

	if (_distanceSquared.x + _distanceSquared.y <= _sumRadiiSquared) {
		collisionVector = *otherObj.getCenter() - corner;
		_collisionCenter = corner;
		otherObj.setCollisionCenter(*otherObj.getCenter());
		return true;
	}

	return false;
}

void GameObject::bounce(GameObject& otherObj, VECTOR2& collisionVector) {
	VECTOR2 vectorDiff = otherObj.getVelocity() - _velocity;
	VECTOR2 collisionUnitVector = collisionVector;
	Graphics::Vector2Normalize(&collisionUnitVector);

	float cUvDotVDiff = Graphics::Vector2Dot(&collisionUnitVector, &vectorDiff);
	float massRatio = 2.0f;
	float thisMass = this->getMass();
	if (thisMass != 0) {
		massRatio *= (otherObj.getMass() / (thisMass + otherObj.getMass()));
	}

	if (massRatio < 0.1f) {
		massRatio = 01.f;
	}

	VECTOR2 cv;
	int count = 10;
	do {
		setX(this->getX() - collisionUnitVector.x);
		setY(this->getY() - collisionUnitVector.y);
	} while (this->isCollidingWith(otherObj, cv) && count);

	// Bounce
	_deltaVelocity += ((massRatio * cUvDotVDiff) * collisionUnitVector);
}
