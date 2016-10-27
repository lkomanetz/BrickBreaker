#include "Image.h"

Image::Image() {
	this->construct();
}

Image::Image(Graphics* pGraphics, int width, int height, int ncols, TextureManager* textureManager) {
	this->construct();
	this->initialize(pGraphics, width, height, ncols, textureManager);
}

Image::~Image() {
}

void Image::update(float frameTime) {
	if (isAnimated()) {
		_animationTimer += frameTime;

		if (_animationTimer > _frameDelay) {
			_animationTimer -= _frameDelay;
			_currentFrame++;

			if (_currentFrame < _startFrame || _currentFrame > _endFrame) {
				if (_loop == true) {
					_currentFrame = _startFrame;
				}
				else {
					_currentFrame = _endFrame;
					_animationComplete = true;
				}
			}

			setRect();
		}
	}
}

Image::Image(const Image& obj) {
	p_textureManager = obj.p_textureManager;
	_initialized = obj._initialized;
	p_graphics = obj.p_graphics;
	_spriteData = obj._spriteData;
	_colorFilter = obj._colorFilter;
	_columns = obj._columns;
	_startFrame = obj._startFrame;
	_endFrame = obj._endFrame;
	_currentFrame = obj._currentFrame;
	_frameDelay = obj._frameDelay;
	_animationTimer = obj._animationTimer;
	_result = obj._result;
	_loop = obj._loop;
	_visible = obj._visible;
	_animationComplete = obj._animationComplete;
}

Image& Image::operator=(const Image& obj) {
	p_textureManager = obj.p_textureManager;
	_initialized = obj._initialized;
	p_graphics = obj.p_graphics;
	_spriteData = obj._spriteData;
	_colorFilter = obj._colorFilter;
	_columns = obj._columns;
	_startFrame = obj._startFrame;
	_endFrame = obj._endFrame;
	_currentFrame = obj._currentFrame;
	_frameDelay = obj._frameDelay;
	_animationTimer = obj._animationTimer;
	_result = obj._result;
	_loop = obj._loop;
	_visible = obj._visible;
	_animationComplete = obj._animationComplete;
	return *this;
}

void Image::construct() {
	_initialized = false;
	_columns = 1;
	_startFrame = 0;
	_endFrame = 0;
	_currentFrame = 0;
	_frameDelay = 1.0;
	_animationTimer = 0.0;
	_visible = true;
	_loop = true;
	_animationComplete = false;
	p_graphics = NULL;
	_colorFilter = GraphicsNS::WHITE;

	_spriteData.width = 2;
	_spriteData.height = 2;
	_spriteData.x = 0.0;
	_spriteData.y = 0.0;
	_spriteData.scale = 1.0;
	_spriteData.angle = 0.0;
	_spriteData.rectangle.left = 0;
	_spriteData.rectangle.top = 0;
	_spriteData.rectangle.right = _spriteData.width;
	_spriteData.rectangle.bottom = _spriteData.height;
	_spriteData.texture = NULL;
	_spriteData.isVerticalFlipped = false;
	_spriteData.isHorizontalFlipped = false;
}

bool Image::initialize(Graphics* graphics, int width, int height, int ncols, TextureManager* textureManager) {
	try {
		p_graphics = graphics;
		p_textureManager = textureManager;
		_spriteData.texture = p_textureManager->getTexture();

		if (width == 0) {
			width = p_textureManager->getWidth();
		}
		_spriteData.width = width;

		if (height == 0) {
			height = p_textureManager->getHeight();
		}
		_spriteData.height = height;

		_columns = ncols;
		if (_columns == 0) {
			_columns = 1;
		}

		_spriteData.rectangle.left = (_currentFrame % _columns) * _spriteData.width;
		_spriteData.rectangle.right = _spriteData.rectangle.left + _spriteData.width;
		_spriteData.rectangle.top = (_currentFrame / _columns) * _spriteData.height;
		_spriteData.rectangle.bottom = _spriteData.rectangle.top + _spriteData.height;
	}
	catch (...) {
		_initialized = false;
		throw GameError(
			GameErrorNS::FATAL_ERROR,
			"Unable to initialize Image."
		);
		return false;
	}
	_initialized = true;
	return true;
}

void Image::setCurrentFrame(int currentFrame) {
	if (currentFrame >= 0) {
		_currentFrame = currentFrame;
		_animationComplete = false;
		setRect();
	}
}

inline void Image::setRect() {
	_spriteData.rectangle.left = (_currentFrame % _columns) * _spriteData.width;
	_spriteData.rectangle.right = _spriteData.rectangle.left + _spriteData.width;
	_spriteData.rectangle.top = (_currentFrame / _columns) * _spriteData.width;
	_spriteData.rectangle.bottom = _spriteData.rectangle.top + _spriteData.height;
}

void Image::draw(COLOR_ARGB color) {
	if (!_visible || p_graphics == NULL) {
		return;
	}

	_spriteData.texture = p_textureManager->getTexture();
	if (color == GraphicsNS::FILTER) {
		p_graphics->drawSprite(_spriteData, _colorFilter);
	}
	else {
		p_graphics->drawSprite(_spriteData, color);
	}
}

void Image::draw(SpriteData sd, COLOR_ARGB color) {
	if (!_visible || p_graphics == NULL) {
		return;
	}

	sd.rectangle = _spriteData.rectangle;
	sd.texture = p_textureManager->getTexture();

	if (color == GraphicsNS::FILTER) {
		p_graphics->drawSprite(sd, _colorFilter);
	}
	else {
		p_graphics->drawSprite(sd, color);
	}
}