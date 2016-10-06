#include "Game.h"

Game::Game() :
	_paused(false),
	_initialized(false),
	p_graphics(NULL) {

	p_input = new Input();
}

void Game::initialize(HWND hwnd) {
	_hwnd = hwnd;
	p_graphics = new Graphics(_hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);
	p_input->initialize(hwnd, false);
	if (QueryPerformanceFrequency(&_timerFrequency) == false) {
		throw GameError(GameErrorNS::FATAL_ERROR, "Error initializing high resolution timer.");
	}

	QueryPerformanceCounter(&_timeStart);
	_initialized = true;
}

Game::~Game() {
	safeDelete(p_graphics);
	deleteAll();
}

HRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (!_initialized) {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:
			p_input->keyDown(wParam);
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP:
			p_input->keyUp(wParam);
			return 0;
		case WM_MOUSEMOVE:
			p_input->mouseIn(lParam);
			return 0;
		case WM_INPUT:
			p_input->mouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN:
			p_input->setMouseLButton(true);
			p_input->mouseIn(lParam);
			return 0;
		case WM_LBUTTONUP:
			p_input->setMouseLButton(false);
			p_input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN:
			p_input->setMouseRButton(true);
			p_input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONUP:
			p_input->setMouseRButton(false);
			p_input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONDOWN:
			p_input->setMouseMButton(true);
			p_input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONUP:
			p_input->setMouseMButton(false);
			p_input->mouseIn(lParam);
			return 0;
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:
			p_input->setMouseXButton(wParam);
			p_input->mouseIn(lParam);
			return 0;
		case WM_MOUSEWHEEL:
			p_input->mouseWheelIn(wParam);
			return 0;
		case WM_DEVICECHANGE:
			p_input->checkControllers();
			return 0;
		case WM_CHAR:
			p_input->keyIn(wParam);
			switch (wParam) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					return 0;
			}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Game::resetAll() {}

void Game::renderGame() {
	if (SUCCEEDED(p_graphics->beginScene())) {
		render();
		p_graphics->endScene();
	}

	handleLostGraphicsDevice();
	p_graphics->showBackBuffer();
}

void Game::run(HWND hwnd) {
	if (p_graphics == NULL) {
		return;
	}

	QueryPerformanceCounter(&_timeEnd);
	_frameTime = (float) (_timeEnd.QuadPart - _timeStart.QuadPart) / (float) _timerFrequency.QuadPart;

	if (_frameTime < MIN_FRAME_TIME) {
		_sleepTime = (DWORD) ((MIN_FRAME_TIME - _frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(_sleepTime);
		timeEndPeriod(1);
		return;
	}

	if (_frameTime > 0.0) {
		_framesPerSecond = (_framesPerSecond * 0.99f) + (0.01f / _frameTime);
	}
	if (_frameTime > MAX_FRAME_TIME) {
		_frameTime = MAX_FRAME_TIME;
	}

	_timeStart = _timeEnd;
	p_input->readControllers();

	if (!_paused) {
		update();
		performAi();
		calculateCollisions();
		p_input->vibrateControllers(_frameTime);
	}

	renderGame();

	p_input->readControllers();
	p_input->clear(InputNS::KEYS_PRESSED);
}

void Game::handleLostGraphicsDevice() {
	_result = p_graphics->getDeviceState();
	if (FAILED(_result)) {
		if (_result == D3DERR_DEVICELOST) {
			Sleep(100);
			return;
		}
		else if (_result == D3DERR_DEVICENOTRESET) {
			releaseAll();
			_result = p_graphics->reset();
			if (FAILED(_result)) {
				return;
			}

			resetAll();
		}
	}
}

void Game::deleteAll() {
	releaseAll();
	safeDelete(p_graphics);
	safeDelete(p_input);
	_initialized = false;
}

void Game::releaseAll() {
}