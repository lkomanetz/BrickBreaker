#pragma once
#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "GameObject.h"

enum class CollisionType {
	None,
	Circle,
	Box,
	RotatedBox
};

class PhysicsObject {
private:
	GameObject* _gameObject;
	CollisionType _type;
	float _radius;
	VECTOR2 _distanceSquared;
	float _sumRadiiSquared;
	RECT _edge;
	VECTOR2 _corners[4];
	VECTOR2 _edge01, _edge03;
	float _edge01Min, _edge01Max, _edge03Min, _edge03Max;
	VECTOR2 _velocity;
	VECTOR2 _deltaVelocity;
	float _mass;
	float _radiusSquared;
	float _force;
	float _gravity;
	bool _rotatedBoxReady;

	bool collideCircle(PhysicsObject& otherObj);
	
public:
	const VECTOR2* getCenter();
	float getRadius() { return _radius; }
	GameObject* getGameObject() { return _gameObject; }
	bool isCollidingWith(PhysicsObject& otherObj);
	CollisionType getType() { return _type; }
};

#endif