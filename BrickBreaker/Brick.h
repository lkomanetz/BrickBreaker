#pragma once
#ifndef _BRICK_H
#define _BRICK_H

#include "GameObject.h"

class Brick : public GameObject {
private:
	const char* _imageLocation = "images/brick.png";
public:
	Brick();
	virtual ~Brick();
};

#endif