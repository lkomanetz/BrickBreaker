#pragma once
#ifndef _BRICKBREAKER_H
#define _BRICKBREAKER_H
#include "Game.h"
#include "GameObject.h"
#include "Brick.h"
#include "Level.h"
#include "Stage.h"
#include "FileReader.h"
#include <vector>

//TODO(Logan) -> use "p" prefix for all pointers
class BrickBreaker : public Game {
private:
	std::vector<Stage*> _stages;
	std::vector<GameObject*> _background;
	Brick*** p_currentLayout;
	UINT _currentStage;
	UINT _currentLevel;

	void loadLevel();
	void loadStagesFromFile();
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