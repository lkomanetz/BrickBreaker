#pragma once
#ifndef _BRICKBREAKER_H
#define _BRICKBREAKER_H
#include "Game.h"
#include "GameObject.h"
#include "Brick.h"
#include "Level.h"
#include "FileReader.h"
#include <vector>

class BrickBreaker : public Game {
private:
	std::vector<GameObject*> _gameObjects;
	Brick*** _objects;
	UINT _stage;

	void loadLevelFromFile();
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