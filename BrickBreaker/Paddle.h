#pragma once
#ifndef _PADDLE_H
#define _PADDLE_H

#include "Engine/GameObject.h"
#include "Engine/Graphics.h"

class Paddle : public GameObject {
private:
	void initialize();
public:
	Paddle();
	Paddle(Game* pGame, TextureManager* pTextureManager);
	void update(float frameTime);
	void doDpadMovement(float frameTime);
	void doThumbstickMovement(float frameTime);
};

#endif