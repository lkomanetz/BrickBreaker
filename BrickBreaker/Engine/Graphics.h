#pragma once
#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

class Graphics;
#include <d3d9.h>
#include <d3dx9.h>
#include "../Constants.h"
#include "GameError.h"

// DirectX pointer types
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D LPDIRECT3D9
#define LP_SPRITE LPD3DXSPRITE
#define LP_TEXTURE LPDIRECT3DTEXTURE9
#define VECTOR2 D3DXVECTOR2

// Color defines
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
	((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

namespace GraphicsNS {
	// Some common colors
	// ARGB numbers range from 0 through 255
	// A = Alpha channel (transparency where 255 is opaque)
	// R = Red, G = Green, B = Blue
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);  // use to specify drawing with colorFilter
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);  // AND with color to get 25% alpha
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);  // AND with color to get 50% alpha
	const COLOR_ARGB BACK_COLOR = NAVY; // background color of game

	enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
}

struct SpriteData {
	int width;
	int height;
	float x;
	float y;
	float scale;
	float angle;
	RECT rectangle;
	LP_TEXTURE texture;
	bool isHorizontalFlipped;
	bool isVerticalFlipped;
};

// Wrapper class for all DirectX interaction
class Graphics {
private:
	/* DirectX specific pointers and variables*/
	LP_3D p_direct3d;
	LP_3DDEVICE p_device3d;
	D3DPRESENT_PARAMETERS _presentParams;
	D3DDISPLAYMODE _displayMode;
	LP_SPRITE p_sprite;

	HRESULT _result;
	HWND _hwnd;
	int _width;
	int _height;
	bool _isFullscreen;
	COLOR_ARGB _backgroundColor;

	void initializePresentationParams();
	bool isAdapterCompatible();
	DWORD getHardwareBehavior();
public:
	Graphics(HWND hwnd, int width, int height, bool fullscreen);
	virtual ~Graphics();

	HRESULT showBackBuffer();
	HRESULT getDeviceState();
	HRESULT reset();
	HRESULT beginScene();
	HRESULT endScene();
	HRESULT loadTexture(const char* filename, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture);
	void dispose();
	void drawSprite(const SpriteData &spriteData, COLOR_ARGB color);
	void spriteBegin() { p_sprite->Begin(D3DXSPRITE_ALPHABLEND); }
	void spriteEnd() { p_sprite->End(); }

	static float Vector2Length(const VECTOR2 *v) { return D3DXVec2Length(v); }
	static float Vector2Dot(const VECTOR2* v1, const VECTOR2* v2) { return D3DXVec2Dot(v1, v2); }
	static void Vector2Normalize(VECTOR2* v) { D3DXVec2Normalize(v, v); }
	static void Vector2Square(VECTOR2* v) { v->x *= v->x; v->y *= v->y; }
	static VECTOR2* Vector2Transform(VECTOR2* v, D3DXMATRIX *m) { return D3DXVec2TransformCoord(v, v, m); }

	LP_3D getDirect3d() { return p_direct3d; }
	LP_3DDEVICE get3dDevice() { return p_device3d; }
	LP_SPRITE getSprite() { return p_sprite; }
};

#endif