#pragma once
#ifndef _BRICK_H
#define _BRICK_H

#include "GameObject.h"
#include "Graphics.h"
#include <string>

enum class BrickType : int {
	NoBrick,
	Normal,
	TwoHit,
	Indestructible
};

class Brick : public GameObject {
private:
	const char* _imageLocation = "images/brick.png";
	BrickType _type;

public:
	Brick();
	Brick(Graphics* pGraphics);
	Brick(const Brick& obj);
	virtual ~Brick();

	Brick& operator=(const Brick& rightObj);

	BrickType getType() const { return _type; }
	void setType(const BrickType newType) { _type = newType; }
};

#endif