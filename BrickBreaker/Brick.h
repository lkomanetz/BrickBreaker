#pragma once
#ifndef _BRICK_H
#define _BRICK_H

#include "Engine/GameObject.h"
#include "Engine/Graphics.h"
#include <string>

enum class BrickType : int {
	NoBrick,
	Normal,
	TwoHit,
	Indestructible
};

class Brick : public GameObject {
private:
	const int _imageHeight = 18;
	const int _imageWidth = 59;
	const int _numberOfColumns = 3;
	BrickType _type;

public:
	Brick();
	Brick(Graphics* pGraphics, TextureManager* textureManager);
	Brick(const Brick& obj);
	virtual ~Brick();

	Brick& operator=(const Brick& rightObj);

	BrickType getType() const { return _type; }
	void setType(const BrickType newType) { _type = newType; }
};

#endif