#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#define WIN32_LEAN_AND_MEAN

#include "Game.h"
#include "Image.h"
#include "Input.h"
#include "Physics.h"
#include <vector>

//TODO(Logan) -> Add physics code possibly into its own separate class so a GameObject can have multiple physics objects.
// The check for physics should return true if colliding, the point of the collision, and the entity it is colliding with.
class GameObject : public Image {
protected:
	VECTOR2 _center;
	Input* p_input;
	UINT _health;
	UINT _imageWidth;
	UINT _imageHeight;
	UINT _numberOfColumns;
	bool _active;
	std::vector<PhysicsObject> _physicsObjects;

public:
	GameObject();
	GameObject(const GameObject& obj);
	virtual ~GameObject() {};

	GameObject& operator=(const GameObject& rightObj);

	bool initialize(Game* pGame, int width, int height, int columnCount, TextureManager* pTextureManager);
	bool isActive() { return _active; }
	bool isCollidingWith(GameObject& otherObj);
	void setHealth(UINT totalHealth) { _health = totalHealth; }
	UINT getHealth() { return _health; }
	const VECTOR2* getCenter();
	std::vector<PhysicsObject> getPhysicsObjects() { return _physicsObjects; }
	void update(float frameTime);
};

#endif