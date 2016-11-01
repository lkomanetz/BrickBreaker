#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#define WIN32_LEAN_AND_MEAN

#include "Game.h"
#include "Image.h"
#include "Input.h"
#include <vector>

namespace gameObjectNS {
	const float GRAVITY = 6.67428e-11f; // Gravitation constant
}

enum class CollisionType {
	None,
	Circle,
	Box,
	RotatedBox
};

class GameObject : public Image {
protected:
	CollisionType _collisionType;
	VECTOR2 _center;
	float _radius;
	VECTOR2 _distanceSquared;
	float _sumRadiiSquared;
	RECT _edge; // For Box and RotatedBox collision detection.  Specifies collision box relative to center of GameObject.
	VECTOR2 _corners[4]; // Used fr RotatedBox collision detection.
	VECTOR2 _edge01, _edge03; // Edges used for projections.
	float _edge01Min, _edge01Max, _edge03Min, _edge03Max; // Min and Max projections
	VECTOR2 _velocity;
	VECTOR2 _deltaVelocity;
	float _mass;
	float _radiusSquared;
	float _force;
	float _gravity;

	Input* p_input;
	UINT _health;
	UINT _imageWidth;
	UINT _imageHeight;
	UINT _numberOfColumns;
	bool _active;

	virtual bool collideCircle(GameObject& otherObj, VECTOR2& collisionVector);

public:
	GameObject();
	GameObject(const GameObject& obj);
	virtual ~GameObject() {};

	GameObject& operator=(const GameObject& rightObj);

	bool initialize(Game* pGame, int width, int height, int columnCount, TextureManager* pTextureManager);
	void update(float frameTime);
	virtual void performAi(float frameTime, GameObject& otherObj);

	bool isActive() { return _active; }
	bool isCollidingWith(GameObject& otherObj, VECTOR2& collisionVector);

	virtual const VECTOR2* getCenter();
	virtual const RECT& getEdge() const { return _edge; }
	virtual const VECTOR2* getCorner(UINT corner) const;
	virtual const VECTOR2 getVelocity() const { return _velocity; }
	virtual float getMass() const { return _mass; }
	virtual float getGravity() const { return _gravity; }
	virtual float getRadius() const { return _radius; }
	virtual UINT getHealth() const { return _health; }
	virtual CollisionType getCollisionType() const { return _collisionType; }

	virtual void setHealth(UINT totalHealth) { _health = totalHealth; }
	virtual void setVelocity(VECTOR2 velocity) { _velocity = velocity; }
	virtual void setDeltaVelocity(VECTOR2 deltaVelocity) { _deltaVelocity = deltaVelocity; }
	virtual void setActive(bool isActive) { _active = isActive; }
	virtual void setMass(float mass) { _mass = mass; }
	virtual void setGravity(float gravity) { _gravity = gravity; }
	virtual void setCollisionRadius(float radius) { _radius = radius; }
};

#endif