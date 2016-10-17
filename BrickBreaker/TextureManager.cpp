#include "TextureManager.h"

TextureManager::TextureManager() :
	p_texture(NULL), 
	_width(0),
	_height(0),
	p_file(NULL),
	p_graphics(NULL),
	_initialized(false) {
}

TextureManager::TextureManager(Graphics* pGraphics, const char* file) :
	p_texture(NULL),
	_width(0),
	_height(0),
	p_file(NULL),
	p_graphics(NULL),
	_initialized(false) {

	this->initialize(pGraphics, file);
}

TextureManager::TextureManager(const TextureManager& obj) {
	_width = obj._width;
	_height = obj._height;
	p_file = obj.p_file;
	p_graphics = obj.p_graphics;
	_initialized = obj._initialized;
	_result = obj._result;
	p_graphics->loadTexture(p_file, TRANSCOLOR, _width, _height, p_texture);
}

TextureManager& TextureManager::operator=(const TextureManager& obj) {
	_width = obj._width;
	_height = obj._height;
	p_file = obj.p_file;
	p_graphics = obj.p_graphics;
	_initialized = obj._initialized;
	_result = obj._result;
	p_graphics->loadTexture(p_file, TRANSCOLOR, _width, _height, p_texture);
	return *this;
}

TextureManager::~TextureManager() {
	safeRelease(p_texture);
}

bool TextureManager::initialize(Graphics* pGraphics, const char* file) {
	try {
		p_graphics = pGraphics;
		p_file = file;

		_result = p_graphics->loadTexture(file, TRANSCOLOR, _width, _height, p_texture);

		if (FAILED(_result)) {
			safeRelease(p_texture);
			return false;
		}
	}
	catch (...) {
		return false;
	}
	_initialized = true;
	return true;
}

void TextureManager::onLostDevice() {
	if (!_initialized) {
		return;
	}

	safeRelease(p_texture);
}

void TextureManager::onResetDevice() {
	if (!_initialized) {
		return;
	}

	p_graphics->loadTexture(p_file, TRANSCOLOR, _width, _height, p_texture);
}