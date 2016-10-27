#pragma once
#ifndef _INPUT_H
#define _INPUT_H
#define WIN32_LEAN_AND_MEAN

class Input;

#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <XInput.h>
#include "../Constants.h"
#include "GameError.h"
#include <limits.h>

// For high-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif

namespace InputNS {
	const int KEYS_ARRAY_LENGTH = 256;
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

const DWORD MAX_CONTROLLERS = 4;
const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (short)(0.20f * 0x7FFF);
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;

const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;

struct ControllerState {
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	float vibrateTimeLeft;
	float vibrateTimeRight;
	bool connected;
};

class Input {
private:
	bool _keysDown[InputNS::KEYS_ARRAY_LENGTH];
	bool _keysPressed[InputNS::KEYS_ARRAY_LENGTH];
	std::string _textIn;
	char _charIn;
	bool _newLine;
	int _mouseX, _mouseY;
	int _mouseRawX, _mouseRawY;
	int _mouseWheel;
	RAWINPUTDEVICE _rid[1]; // For the high definition mouse
	bool _mouseCaptured;
	bool _mouseLButton;
	bool _mouseMButton;
	bool _mouseRButton;
	bool _mouseX1Button;
	bool _mouseX2Button;
	ControllerState _controllers[MAX_CONTROLLERS];
	short _triggerDeadZone;
	short _thumbstickDeadZone;

public:
	Input();
	virtual ~Input();
	void initialize(HWND hwnd, bool capture);
	void checkControllers();
	void keyDown(WPARAM key);
	void keyUp(WPARAM key);
	void keyIn(WPARAM key);
	bool isKeyDown(UCHAR key);
	bool wasKeyPressed(UCHAR key) const;
	bool anyKeyPressed() const;
	std::string getTextIn() { return _textIn; }
	char getCharIn() const { return _charIn; }
	void clearKeyPress(UCHAR key);
	void clearTextIn() { _textIn.clear(); }
	void clearAll() { clear(InputNS::KEYS_MOUSE_TEXT); }
	void clear(UCHAR inputToClear);
	void mouseIn(LPARAM lParam);
	void mouseRawIn(LPARAM lParam);
	void mouseWheelIn(WPARAM wParam);
	void setMouseLButton(bool isClicked) { _mouseLButton = isClicked; }
	void setMouseRButton(bool isClicked) { _mouseRButton = isClicked; }
	void setMouseMButton(bool isClicked) { _mouseMButton = isClicked; }
	void setMouseXButton(WPARAM wParam);
	void setThumbstickDeadzone(short dz) { _thumbstickDeadZone = abs(dz); }
	void setTriggerDeadzone(BYTE dz) { _triggerDeadZone = dz; }
	short getThumbstickDeadzone() { return _thumbstickDeadZone; }
	BYTE getTriggerDeadzone() { return static_cast<BYTE>(_triggerDeadZone); }
	int getMouseX() const { return _mouseX; }
	int getMouseY() const { return _mouseY; }
	int getMouseRawX() const { return _mouseRawX; }
	int getMouseRawY() const { return _mouseRawY; }
	bool getMouseLButton() const { return _mouseLButton; }
	bool getMouseMButton() const { return _mouseMButton; }
	bool getMouseRButton() const { return _mouseRButton; }
	bool getMouseX1Button() const { return _mouseX1Button; }
	bool getMouseX2Button() const { return _mouseX2Button; }
	void readControllers();
	const ControllerState* getControllerState(UINT controller);
	const WORD getGamepadButtons(UINT controller);
	bool isGamepadButtonPressed(UINT controller, DWORD button);
	BYTE getGamepadLeftTrigger(UINT controller);
	BYTE getGamepadRightTrigger(UINT controller);
	SHORT getGamepadThumbX(UINT controller, DWORD thumbStick);
	SHORT getGamepadThumbY(UINT controller, DWORD thumbStick);
	void gamepadVibrateLeft(UINT controller, WORD speed, float seconds);
	void gamepadVibrateRight(UINT controller, WORD speed, float seconds);
	void vibrateControllers(float frameTime);
};

#endif