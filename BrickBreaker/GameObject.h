#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Graphics.h"
#include "TextureManager.h"

class GameObject {
private:
	Graphics* p_graphics;
	TextureManager* p_textureManager;
	SpriteData _spriteData;
	COLOR_ARGB _colorFilter;
	int _columns;
	int _startFrame;
	int _endFrame;
	int _currentFrame;
	float _frameDelay;
	float _animationTimer;
	HRESULT _result;
	bool _loop;
	bool _visible;
	bool _initialized;
	bool _animationComplete;

protected:
	bool initialize(Graphics* p_graphics, int width, int height, int ncols, const char* fileLocation);
	void construct();
	void copyData(const GameObject& obj);

public:
	GameObject();
	GameObject(Graphics* pGraphics, int width, int height, int ncols, const char* fileLocation);
	GameObject(const GameObject& obj);
	virtual ~GameObject();

	virtual void draw(COLOR_ARGB color = GraphicsNS::WHITE);
	virtual void draw(SpriteData spriteData, COLOR_ARGB color = GraphicsNS::WHITE);
	virtual void update(float frameTime);
	virtual void flipHorizontal(bool isFlipped) { _spriteData.isHorizontalFlipped = isFlipped; }
	virtual void flipVertical(bool isFlipped) { _spriteData.isVerticalFlipped = isFlipped; }
	const virtual SpriteData& getSpriteInfo() { return _spriteData; }
	virtual bool getVisible() { return _visible; }
	virtual float getScale() { return _spriteData.scale; }
	virtual float getX() { return _spriteData.x; }
	virtual float getY() { return _spriteData.y; }
	virtual int getWidth() { return _spriteData.width; }
	virtual int getHeight() { return _spriteData.height; }
	virtual float getCenterX() { return _spriteData.x + _spriteData.width / 2 * getScale(); }
	virtual float getCenterY() { return _spriteData.y + _spriteData.height / 2 * getScale(); }
	virtual float getDegrees() { return _spriteData.angle * (180.0f / (float) PI); }
	virtual float getRadians() { return _spriteData.angle; }
	virtual float getFrameDelay() { return _frameDelay; }
	virtual int getStartFrame() { return _startFrame; }
	virtual int getEndFrame() { return _endFrame; }
	virtual int getCurrentFrame() { return _currentFrame; }
	virtual RECT getSpriteDataRect() { return _spriteData.rectangle; }
	virtual bool getAnimationComplete() { return _animationComplete; }
	virtual COLOR_ARGB getColorFilter() { return _colorFilter; }
	virtual void setX(float x) { _spriteData.x = x; }
	virtual void setY(float y) { _spriteData.y = y; }
	virtual void setScale(float scale) { _spriteData.scale = scale; }
	virtual void setDegrees(float degrees) { _spriteData.angle = degrees * ((float) PI / 180.0f); }
	virtual void setRadians(float radians) { _spriteData.angle = radians; }
	virtual void setVisible(bool isVisible) { _visible = isVisible; }
	virtual void setFrameDelay(float delay) { _frameDelay = delay; }
	virtual void setFrames(int start, int end) { _startFrame = start; _endFrame = end; }
	virtual void setCurrentFrame(int current);
	virtual void setRect();
	virtual void setSpriteDataRect(RECT rectangle) { _spriteData.rectangle = rectangle; }
	virtual void setLoop(bool loop) { _loop = loop; }
	virtual void setAnimationComplete(bool isComplete) { _animationComplete = isComplete; }
	virtual void setColorFilter(COLOR_ARGB color) { _colorFilter = color; }
	virtual void setTextureManager(TextureManager* tm) { p_textureManager = tm; }
	virtual TextureManager* getTextureManager() { return p_textureManager; }
	virtual GameObject& operator=(const GameObject& obj);
};

#endif
