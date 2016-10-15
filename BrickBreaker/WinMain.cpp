#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Graphics.h"
#include "BrickBreaker.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND&, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;
BrickBreaker* game;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MSG msg;
	HWND hwnd = NULL;

	game = new BrickBreaker();
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow)) {
		return 1;
	}

	try {
		game->initialize(hwnd);
		bool done = false;
		while (!done) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					done = true;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				game->run(hwnd);
			}
		}
		safeDelete(game);
	}
	catch (const GameError& err) {
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	catch (...) {
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occurred in game.", "Error", MB_OK);
	}

	safeDelete(game);
	return msg.wParam;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return game->messageHandler(hWnd, msg, wParam, lParam);
}

bool CreateMainWindow(HWND& hWnd, HINSTANCE hInstance, int nCmdShow) {
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;

	if (RegisterClassEx(&wcx) == 0) {
		return false;
	}

	DWORD style;
	if (FULLSCREEN) {
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else {
		style = WS_OVERLAPPEDWINDOW;
	}

	hWnd = CreateWindow(
		CLASS_NAME,
		GAME_TITLE,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		GAME_WIDTH,
		GAME_HEIGHT,
		(HWND) NULL,
		(HMENU) NULL,
		hInstance,
		(LPVOID) NULL
	);

	if (!hWnd) {
		return false;
	}

	if (!FULLSCREEN) {
		RECT clientRect;
		GetClientRect(hWnd, OUT &clientRect);
		MoveWindow(
			hWnd,
			0,
			0,
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom),
			TRUE
		);
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return true;
}