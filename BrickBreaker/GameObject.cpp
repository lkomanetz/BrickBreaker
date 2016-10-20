#include "GameObject.h"

GameObject::GameObject() {
	this->construct();
}

GameObject::GameObject(Graphics* pGraphics, int width, int height, int ncols, const char* fileLocation) {
	this->construct();
	this->initialize(pGraphics, width, height, ncols, fileLocation);
}

GameObject::~GameObject() {
}

void GameObject::update(float frameTime) {
	if ((_endFrame - _startFrame) == 0) {
		_animationTimer += frameTime;

		if (_animationTimer > _frameDelay) {
			_animationTimer -= _frameDelay;
			_currentFrame++;

			if (_currentFrame < _startFrame ||
				_currentFrame > _endFrame) {

				if (_loop) {
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

GameObject::GameObject(const GameObject& obj) {
	_textureManager = obj._textureManager;
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

GameObject& GameObject::operator=(const GameObject& obj) {
	_textureManager = obj._textureManager;
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

void GameObject::construct() {
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

bool GameObject::initialize(Graphics* graphics, int width, int height, int ncols, const char* fileLocation) {
	try {
		p_graphics = graphics;
		_textureManager = TextureManager(graphics, fileLocation);
		_spriteData.texture = _textureManager.getTexture();

		if (width == 0) {
			width = _textureManager.getWidth();
		}
		_spriteData.width = width;

		if (height == 0) {
			height = _textureManager.getHeight();
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
			"Unable to initialize image for file '" + std::string(fileLocation) + "'"
		);
		return false;
	}
	_initialized = true;
	return true;
}

void GameObject::setCurrentFrame(int currentFrame) {
	if (currentFrame >= 0) {
		_currentFrame = currentFrame;
		_animationComplete = false;
		setRect();
	}
}

inline void GameObject::setRect() {
	_spriteData.rectangle.left = (_currentFrame % _columns) * _spriteData.width;
	_spriteData.rectangle.right = _spriteData.rectangle.left + _spriteData.width;
	_spriteData.rectangle.top = (_currentFrame / _columns) * _spriteData.width;
	_spriteData.rectangle.bottom = _spriteData.rectangle.top + _spriteData.height;
}

void GameObject::draw(COLOR_ARGB color) {
	if (!_visible || p_graphics == NULL) {
		return;
	}

	_spriteData.texture = _textureManager.getTexture();
	if (color == GraphicsNS::FILTER) {
		p_graphics->drawSprite(_spriteData, _colorFilter);
	}
	else {
		p_graphics->drawSprite(_spriteData, color);
	}
}

void GameObject::draw(SpriteData sd, COLOR_ARGB color) {
	if (!_visible || p_graphics == NULL) {
		return;
	}

	sd.rectangle = _spriteData.rectangle;
	sd.texture = _textureManager.getTexture();

	if (color == GraphicsNS::FILTER) {
		p_graphics->drawSprite(sd, _colorFilter);
	}
	else {
		p_graphics->drawSprite(sd, color);
	}
}