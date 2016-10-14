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

class BrickBreaker : public Game {
private:
	std::vector<Stage*> _stages;
	std::vector<GameObject*> _background;
	Level* _level;
	Stage* _stage;
	UINT _currentStage;
	UINT _currentLevel;

	void loadLevel();
	void loadStagesFromFile();
	void renderLevel();
	Stage* getStage(int stageNumber);

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