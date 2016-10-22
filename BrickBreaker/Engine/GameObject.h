#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#define WIN32_LEAN_AND_MEAN

#include "Image.h"

class GameObject : public Image {
protected:
	UINT _health;
	UINT _imageWidth;
	UINT _imageHeight;
	UINT _numberOfColumns;

public:
	GameObject();
	GameObject(const GameObject& obj);
	virtual ~GameObject() {};

	GameObject& operator=(const GameObject& rightObj);
	void setHealth(UINT totalHealth) { _health = totalHealth; }
	UINT getHealth() { return _health; }
};

#endif