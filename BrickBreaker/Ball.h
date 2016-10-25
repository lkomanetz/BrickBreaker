#pragma once
#ifndef _BALL_H
#define _BALL_H

#include "Engine/GameObject.h"
#include "Engine/Graphics.h"

class GameObject;
class Graphics;

class Ball : public GameObject {
private:
	void initialize();
public:
	Ball();
	Ball(Graphics* pGraphics, TextureManager* pTextureManager);
};

#endif
