#pragma once
#ifndef _CONSTANTS_H
#define _CONSTANTS_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "Engine/Input.h"
#define OUT // Used to define out parameters versus IN parameters
#define TRANSCOLOR SETCOLOR_ARGB(0, 255, 0, 255) // Magenta transparent color
#define assert(expression) { if (!expression) { *(int*)0 = 0; } }

//Graphic images
const char NEBULA_IMAGE [] = "images\\orion.jpg";
const char PLANET_IMAGE [] = "images\\planet.png";
const char BRICK_IMAGE [] = "images\\bricks.png";
const char PADDLE_IMAGE[] = "images\\paddle.png";
const char BALL_IMAGE[] = "images\\ball.png";

// Game
const double PI = 3.14159265;
const char CLASS_NAME[] = "WinMain";
const char GAME_TITLE [] = "Block Breaker";
const bool FULLSCREEN = false;
const UINT GAME_WIDTH = 640;
const UINT GAME_HEIGHT = 480;
const float FRAME_RATE = 200.0f;
const float MAX_PADDLE_SPEED = 250.0f;
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE; // Minimum desired time for one frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_TIME; // Maximum time used in game calculations

// Key Mappings
const UCHAR NEXT_MAP = VK_LEFT;
const UCHAR MOVE_PADDLE_LEFT = 0x41; //Keyboard_A
const UCHAR MOVE_PADDLE_RIGHT = 0x44; //Keyboard_D

template <typename T>
inline void safeRelease(T& ptr) {
	if (ptr) {
		ptr->Release();
		ptr = NULL;
	}
}

template <typename T>
inline void safeDelete(T& ptr) {
	if (ptr) {
		delete ptr;
		ptr = NULL;
	}
}

template <typename T>
inline void safeDeleteArray(T& ptr) {
	if (ptr) {
		delete [] ptr;
		ptr = NULL;
	}
}

#endif
