#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#define WIN32_LEAN_AND_MEAN

#include "Game.h"
#include "Image.h"
#include "Input.h"

class GameObject : public Image {
protected:
	Input* p_input;
	UINT _health;
	UINT _imageWidth;
	UINT _imageHeight;
	UINT _numberOfColumns;
	bool _active;

public:
	GameObject();
	GameObject(const GameObject& obj);
	virtual ~GameObject() {};

	GameObject& operator=(const GameObject& rightObj);

	bool initialize(Game* pGame, int width, int height, int columnCount, TextureManager* pTextureManager);
	bool isActive() { return _active; }
	void setHealth(UINT totalHealth) { _health = totalHealth; }
	UINT getHealth() { return _health; }
	void update(float frameTime);
};

#endif