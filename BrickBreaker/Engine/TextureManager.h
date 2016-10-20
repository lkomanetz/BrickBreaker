#pragma once
#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H
#define WIN32_LEAN_AND_MEAN

#include "Graphics.h"
#include "../Constants.h"

class TextureManager {
private:
	UINT _width;
	UINT _height;
	LP_TEXTURE p_texture;
	const char* p_file;
	Graphics* p_graphics;
	bool _initialized;
	HRESULT _result;

	virtual bool initialize(Graphics* pGraphics, const char* file);
public:
	TextureManager();
	TextureManager(Graphics* pGraphics, const char* file);
	TextureManager(const TextureManager& obj);
	virtual ~TextureManager();
	
	LP_TEXTURE getTexture() { return p_texture; }
	UINT getWidth() { return _width; }
	UINT getHeight() { return _height; }
	virtual void onLostDevice();
	virtual void onResetDevice();
	TextureManager& operator=(const TextureManager& obj);
};

#endif