#pragma once
#ifndef _BRICKBREAKER_H
#define _BRICKBREAKER_H
#include "Engine/Game.h"
#include "Engine/Image.h"
#include "Brick.h"
#include "Ball.h"
#include "Paddle.h"
#include "Level.h"
#include "Stage.h"
#include "FileReader.h"
#include <vector>

namespace brickBreakerNS {
	const int ROW_COUNT = 11;
	const int COLUMN_COUNT = 8;
}

//TODO(Logan) -> Figure out a way to store a list of all game objects to iterate for updating.
class BrickBreaker : public Game {
private:
	std::vector<Brick> _bricks;
	Paddle _paddle;
	Ball _gameBall;
	TextureManager _ballTexture;
	TextureManager _paddleTexture;
	TextureManager _brickTextures;
	TextureManager _nebulaTexture;
	TextureManager _planetTexture;
	std::vector<Image> _background;
	std::vector<Stage> _stages;
	Brick** _currentLayout;
	Stage* p_currentStage;
	UINT _currentStageId;
	UINT _currentLevelId;
	Level* p_currentLevel;

	void loadLevel();
	void loadStagesFromFile();
	void renderLayout();
	void destructCurrentLayout();
	void showWinScreen();
	Level* getNextLevel();
	Stage* getStage(int stageNumber);
	std::vector<Image> getBackground();

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
