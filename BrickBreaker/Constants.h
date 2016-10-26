#pragma once
#ifndef _CONSTANTS_H
#define _CONSTANTS_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#define OUT // Used to define out parameters versus IN parameters
#define TRANSCOLOR SETCOLOR_ARGB(0, 255, 0, 255) // Magenta transparent color

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
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE; // Minimum desired time for one frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_TIME; // Maximum time used in game calculations

// Key Mappings
//TODO(Logan) -> Add key mappings for keyboard left and right arrows for the paddle.
//TODO(Logan) -> Add key mappings for gamepad input.
//NOTE(Logan) -> Might need to do something like const UCHAR PADDLE_LEFT = VK_LEFT | GAMEPAD_DPAD_LEFT;
// The idea is that PADDLE_LEFT maps to both keyboard left arrow and gamepad dpad left button
const UCHAR NEXT_MAP = VK_LEFT;

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
