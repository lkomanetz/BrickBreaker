#pragma once
#ifndef _BRICKBREAKER_H
#define _BRICKBREAKER_H
#include "Game.h"
#include "GameObject.h"
#include <vector>

class BrickBreaker : public Game {
private:
	//TODO (Logan) -> Change array to a vector
	GameObject* _gameObjects [2];

public:
	BrickBreaker();
	virtual ~BrickBreaker();
	void initialize(HWND hwnd);
	void update();
	void performAi();
	void render();
	void calculateCollisions();
	void releaseAll();
	void resetAll();
};

#endif