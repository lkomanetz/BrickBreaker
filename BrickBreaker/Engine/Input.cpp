#include "Input.h"

Input::Input() {
	// Key data
	for (UINT i = 0; i < InputNS::KEYS_ARRAY_LENGTH; i++) {
		_keysDown[i] = false;
		_keysPressed[i] = false;
	}

	// Text data
	_newLine = true;
	_textIn = "";
	_charIn = 0;

	// Mouse data
	_mouseX = 0;
	_mouseY = 0;
	_mouseRawX = 0;
	_mouseRawY = 0;
	_mouseLButton = false;
	_mouseMButton = false;
	_mouseRButton = false;
	_mouseX1Button = false;
	_mouseX2Button = false;

	// Controllers
	for (UINT i = 0; i < MAX_CONTROLLERS; i++) {
		_controllers[i].vibrateTimeLeft = 0;
		_controllers[i].vibrateTimeRight = 0;
	}
}

Input::~Input() {
	if (_mouseCaptured) {
		ReleaseCapture();
	}
}

void Input::initialize(HWND hwnd, bool capture) {
	try {
		_mouseCaptured = capture;
		_rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		_rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		_rid[0].dwFlags = RIDEV_INPUTSINK;
		_rid[0].hwndTarget = hwnd;
		RegisterRawInputDevices(_rid, 1, sizeof(_rid[0]));

		if (_mouseCaptured) {
			SetCapture(hwnd);
		}

		ZeroMemory(_controllers, sizeof(ControllerState) * MAX_CONTROLLERS);
		checkControllers();
	}
	catch (...) {
		throw GameError(GameErrorNS::FATAL_ERROR, "Error initializing input system.");
	}
}

void Input::checkControllers() {
	DWORD result;
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
		result = XInputGetState(i, &_controllers[i].state);
		(result == ERROR_SUCCESS) ? _controllers[i].connected = true : _controllers[i].connected = false;
	}
}

void Input::clear(UCHAR inputToClear) {
	if (inputToClear & InputNS::KEYS_DOWN) {
		for (UINT i = 0; i < InputNS::KEYS_ARRAY_LENGTH; i++) {
			_keysDown[i] = false;
		}
	}
	if (inputToClear & InputNS::KEYS_PRESSED) {
		for (UINT i = 0; i < InputNS::KEYS_ARRAY_LENGTH; i++) {
			_keysPressed[i] = false;
		}
	}
	if (inputToClear & InputNS::MOUSE) {
		_mouseX = 0;
		_mouseY = 0;
		_mouseRawX = 0;
		_mouseRawY = 0;
	}
	if (inputToClear & InputNS::TEXT_IN) {
		clearTextIn();
	}
}

void Input::keyDown(WPARAM key) {
	if (key < InputNS::KEYS_ARRAY_LENGTH) {
		_keysDown[key] = true;
		_keysPressed[key] = true;
	}
}

void Input::keyUp(WPARAM key) {
	if (key < InputNS::KEYS_ARRAY_LENGTH) {
		_keysDown[key] = false;
	}
}

void Input::keyIn(WPARAM key) {
	if (_newLine) {
		_textIn.clear();
		_newLine = false;
	}

	if (key == '\b') {
		if (_textIn.length() > 0) {
			_textIn.erase(_textIn.size() - 1);
		}
	}
	else {
		_textIn += key;
		_charIn = key;
	}

	if ((char) key == '\r') {
		_newLine = true;
	}
}

bool Input::isKeyDown(UCHAR key) {
	if (key < InputNS::KEYS_ARRAY_LENGTH) {
		return _keysDown[key];
	}
	else {
		return false;
	}
}

bool Input::wasKeyPressed(UCHAR key) const {
	if (key < InputNS::KEYS_ARRAY_LENGTH) {
		return _keysPressed[key];
	}
	else {
		return false;
	}
}

bool Input::anyKeyPressed() const {
	for (UINT i = 0; i < InputNS::KEYS_ARRAY_LENGTH; i++) {
		if (_keysPressed[i] == true) {
			return true;
		}
	}
	return false;
}

void Input::clearKeyPress(UCHAR key) {
	if (key < InputNS::KEYS_ARRAY_LENGTH) {
		_keysPressed[key] = false;
	}
}

void Input::mouseIn(LPARAM lParam) {
	_mouseX = GET_X_LPARAM(lParam);
	_mouseY = GET_Y_LPARAM(lParam);
}

void Input::mouseRawIn(LPARAM lParam) {
	UINT dwSize = 40;
	static BYTE lpb[40];

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
	RAWINPUT* rawInput = (RAWINPUT*) lpb;

	if (rawInput->header.dwType == RIM_TYPEMOUSE) {
		_mouseRawX = rawInput->data.mouse.lLastX;
		_mouseRawY = rawInput->data.mouse.lLastY;
	}
}

void Input::mouseWheelIn(WPARAM wParam) {
	_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
}

void Input::setMouseXButton(WPARAM wParam) {
	_mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
	_mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
}

void Input::readControllers() {
	DWORD result;
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
		if (_controllers[i].connected) {
			result = XInputGetState(i, &_controllers[i].state);

			if (result == ERROR_DEVICE_NOT_CONNECTED) {
				_controllers[i].connected = false;
			}
		}
	}
}

const ControllerState* Input::getControllerState(UINT controller) {
	if (controller > MAX_CONTROLLERS - 1) {
		controller = MAX_CONTROLLERS - 1;
	}
	return &_controllers[controller];
}

const WORD Input::getGamepadButtons(UINT controller) {
	if (controller > MAX_CONTROLLERS - 1) {
		controller = MAX_CONTROLLERS - 1;
	}
	return _controllers[controller].state.Gamepad.wButtons;
}

bool Input::isGamepadButtonPressed(UINT controller, DWORD button) {
	if (controller > MAX_CONTROLLERS - 1) {
		controller = MAX_CONTROLLERS - 1;
	}

	return bool((_controllers[controller].state.Gamepad.wButtons & button) != 0);
}

BYTE Input::getGamepadLeftTrigger(UINT controller) {
	if (controller > MAX_CONTROLLERS - 1) {
		controller = MAX_CONTROLLERS - 1;
	}
	BYTE value = _controllers[controller].state.Gamepad.bLeftTrigger;
	if (value > _triggerDeadZone) {
		value = (value - _triggerDeadZone) * 255 / (255 - _triggerDeadZone);
	}
	else {
		value = 0;
	}

	return value;
}

BYTE Input::getGamepadRightTrigger(UINT controller) {
	if (controller > MAX_CONTROLLERS - 1) {
		controller = MAX_CONTROLLERS - 1;
	}
	BYTE value = _controllers[controller].state.Gamepad.bRightTrigger;
	if (value > _triggerDeadZone) {
		value = (value - _triggerDeadZone) * 255 / (255 - _triggerDeadZone);
	}
	else {
		value = 0;
	}

	return value;
}

SHORT Input::getGamepadThumbX(UINT controller, DWORD thumbStick) {
	if (controller > MAX_CONTROLLERS - 1) {
		controller = MAX_CONTROLLERS - 1;
	}

	if (!(thumbStick & GAMEPAD_LEFT_THUMB) && !(thumbStick & GAMEPAD_RIGHT_THUMB)) {
		throw GameError(GameErrorNS::FATAL_ERROR, "Incorrect thumb stick provided.");
	}

	int x = 0;
	if (thumbStick & GAMEPAD_LEFT_THUMB) {
		x = _controllers[controller].state.Gamepad.sThumbLX;
	}
	else {
		x = _controllers[controller].state.Gamepad.sThumbRX;
	}

	if (x > _thumbstickDeadZone) {
		x = (x - _thumbstickDeadZone) * 32767 / (32767 - _thumbstickDeadZone);
	}
	else if (x < -_thumbstickDeadZone) {
		x = (x + _thumbstickDeadZone) * 32767 / (32767 - _thumbstickDeadZone);
	}
	else {
		x = 0;
	}

	return static_cast<SHORT>(x);
}

SHORT Input::getGamepadThumbY(UINT controller, DWORD thumbStick) {
	if (controller > MAX_CONTROLLERS - 1) {
		controller = MAX_CONTROLLERS - 1;
	}

	if (!(thumbStick & GAMEPAD_LEFT_THUMB) && !(thumbStick & GAMEPAD_RIGHT_THUMB)) {
		throw GameError(GameErrorNS::FATAL_ERROR, "Incorrect thumb stick provided.");
	}

	int y = 0;
	if (thumbStick & GAMEPAD_LEFT_THUMB) {
		y = _controllers[controller].state.Gamepad.sThumbLY;
	}
	else {
		y = _controllers[controller].state.Gamepad.sThumbRY;
	}

	if (y > _thumbstickDeadZone) {
		y = (y - _thumbstickDeadZone) * 32767 / (32767 - _thumbstickDeadZone);
	}
	else if (y < -_thumbstickDeadZone) {
		y = (y + _thumbstickDeadZone) * 32767 / (32767 - _thumbstickDeadZone);
	}
	else {
		y = 0;
	}

	return static_cast<SHORT>(y);
}

void Input::gamepadVibrateLeft(UINT controller, WORD speed, float seconds) {
	if (controller > MAX_CONTROLLERS - 1) {
		controller = MAX_CONTROLLERS - 1;
	}

	_controllers[controller].vibration.wLeftMotorSpeed = speed;
	_controllers[controller].vibrateTimeLeft = seconds;
}

void Input::gamepadVibrateRight(UINT controller, WORD speed, float seconds) {
	if (controller > MAX_CONTROLLERS - 1) {
		controller = MAX_CONTROLLERS - 1;
	}

	_controllers[controller].vibration.wRightMotorSpeed = speed;
	_controllers[controller].vibrateTimeRight = seconds;
}

void Input::vibrateControllers(float frameTime) {
	for (UINT i = 0; i < MAX_CONTROLLERS; i++) {
		if (_controllers[i].connected) {
			_controllers[i].vibrateTimeLeft -= frameTime;
			if (_controllers[i].vibrateTimeLeft < 0) {
				_controllers[i].vibrateTimeLeft = 0;
				_controllers[i].vibration.wLeftMotorSpeed = 0;
			}
			_controllers[i].vibrateTimeRight -= frameTime;
			if (_controllers[i].vibrateTimeRight < 0) {
				_controllers[i].vibrateTimeRight = 0;
				_controllers[i].vibration.wRightMotorSpeed = 0;
			}

			XInputSetState(i, &_controllers[i].vibration);
		}
	}
}