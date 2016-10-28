#pragma once
#ifndef _GAME_H
#define _GAME_H
#include <Windows.h>
#include <mmsystem.h>
#include "Graphics.h"
#include "../Constants.h"
#include "GameError.h"
#include "Input.h"

class Game {
protected:
	Graphics* p_graphics;
	Input* p_input;
	HWND _hwnd;
	HRESULT _result;
	LARGE_INTEGER _timeStart;
	LARGE_INTEGER _timeEnd;
	LARGE_INTEGER _timerFrequency;
	float _frameTime;
	float _framesPerSecond;
	DWORD _sleepTime;
	bool _paused;
	bool _initialized;

public:
	Game();
	virtual ~Game();
	virtual void deleteAll();
	virtual void initialize(HWND hwnd);
	virtual void handleLostGraphicsDevice();
	virtual void releaseAll();
	virtual void renderGame();
	virtual void resetAll();
	virtual void run(HWND);
	virtual void render() = 0;
	virtual void update() = 0;
	virtual void performAi() = 0;
	virtual void calculateCollisions() = 0;
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Input* getInput() { return p_input; }
	Graphics* getGraphics() { return p_graphics; }
};
#endif