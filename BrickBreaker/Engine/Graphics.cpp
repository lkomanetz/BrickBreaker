#include "Graphics.h"

Graphics::Graphics(HWND hwnd, int width, int height, bool fullscreen) :
	_hwnd(hwnd),
	_width(width),
	_height(height),
	_isFullscreen(fullscreen) {

	_backgroundColor = SETCOLOR_ARGB(255, 0, 0, 128);
	p_direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (p_direct3d == NULL) {
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error Initialize D3D"));
	}

	initializePresentationParams();
	if (fullscreen) {
		if (isAdapterCompatible()) {
			_presentParams.FullScreen_RefreshRateInHz = _displayMode.RefreshRate;
		}
		else {
			throw GameError(
				GameErrorNS::FATAL_ERROR,
				"The graphics device doesn't support the specified resolution and/or format."
			);
		}
	}

	DWORD behavior = getHardwareBehavior();
	_result = p_direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behavior,
		&_presentParams,
		OUT &p_device3d
	);

	if (FAILED(_result)) {
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error created Direct3D device."));
	}

	_result = D3DXCreateSprite(p_device3d, &p_sprite);
	if (FAILED(_result)) {
		throw GameError(GameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite.");
	}
}

Graphics::~Graphics() {
	dispose();
}

void Graphics::initializePresentationParams() {
	try {
		ZeroMemory(&_presentParams, sizeof(_presentParams));
		_presentParams.BackBufferWidth = GAME_WIDTH;
		_presentParams.BackBufferHeight = GAME_HEIGHT;
		if (FULLSCREEN) {
			_presentParams.BackBufferFormat = D3DFMT_X8R8G8B8;
		}
		else {
			_presentParams.BackBufferFormat = D3DFMT_UNKNOWN;
		}

		_presentParams.BackBufferCount = 1;
		_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		_presentParams.hDeviceWindow = _hwnd;
		_presentParams.Windowed = (!FULLSCREEN);
		_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch (...) {
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing D3D presentation parameters."));
	}
}

HRESULT Graphics::beginScene() {
	_result = E_FAIL;
	if (p_device3d == NULL) {
		return _result;
	}

	p_device3d->Clear(0, NULL, D3DCLEAR_TARGET, _backgroundColor, 1.0f, 0);
	_result = p_device3d->BeginScene();
	return _result;
}

HRESULT Graphics::endScene() {
	_result = E_FAIL;

	if (p_device3d) {
		_result = p_device3d->EndScene();
	}

	return _result;
}

HRESULT Graphics::getDeviceState() {
	_result = E_FAIL;
	if (p_device3d == NULL) {
		return _result;
	}

	_result = p_device3d->TestCooperativeLevel();
	return _result;
}

HRESULT Graphics::reset() {
	_result = E_FAIL;
	initializePresentationParams();
	_result = p_device3d->Reset(&_presentParams);
	return _result;
}

DWORD Graphics::getHardwareBehavior() {
	D3DCAPS9 caps;
	DWORD behavior;
	_result = p_direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	/*
	 If the device doesn't support HW T&L or doesn't support 1.1 vertex shaders in the hardware,
	 then switch to software vertex processing
	 */
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1)) {

		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else {
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	return behavior;
}

HRESULT Graphics::showBackBuffer() {
	HRESULT result = E_FAIL;
	result = p_device3d->Present(NULL, NULL, NULL, NULL);

	return result;
}

bool Graphics::isAdapterCompatible() {
	UINT modes = p_direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, _presentParams.BackBufferFormat);

	for (UINT mode = 0; mode < modes; mode++) {
		_result = p_direct3d->EnumAdapterModes(
			D3DADAPTER_DEFAULT,
			_presentParams.BackBufferFormat,
			mode,
			OUT &_displayMode
		);

		if (_displayMode.Height == _presentParams.BackBufferHeight &&
			_displayMode.Width == _presentParams.BackBufferWidth &&
			_displayMode.RefreshRate >= _presentParams.FullScreen_RefreshRateInHz) {

			return true;
		}
	}

	return false;
}

void Graphics::dispose() {
	safeRelease(p_device3d);
	safeRelease(p_direct3d);
}

HRESULT Graphics::loadTexture(
	const char* filename,
	COLOR_ARGB transparentColor,
	UINT& width,
	UINT& height,
	LP_TEXTURE& texture
) {
	D3DXIMAGE_INFO imageInfo;
	_result = E_FAIL;

	try {
		if (filename == NULL) {
			return D3DERR_INVALIDCALL;
		}

		_result = D3DXGetImageInfoFromFile(filename, &imageInfo);
		if (_result != D3D_OK) {
			return _result;
		}

		width = imageInfo.Width;
		height = imageInfo.Height;

		_result = D3DXCreateTextureFromFileEx(
			p_device3d,
			filename,
			imageInfo.Width,
			imageInfo.Height,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			transparentColor,
			&imageInfo,
			NULL,
			&texture
		);
	}
	catch (...) {
		throw GameError(GameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture");
	}

	return _result;
}

void Graphics::drawSprite(const SpriteData& spriteData, COLOR_ARGB color) {
	if (spriteData.texture == NULL) {
		return;
	}

	D3DXVECTOR2 spriteCenter = D3DXVECTOR2(
		(float) (spriteData.width / 2 * spriteData.scale),
		(float) (spriteData.height / 2 * spriteData.scale)
	);

	D3DXVECTOR2 translate = D3DXVECTOR2(spriteData.x, spriteData.y);
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

	if (spriteData.isHorizontalFlipped) {
		scaling.x *= -1;
		spriteCenter.x -= (float) (spriteData.width * spriteData.scale);
		translate.x += (float) (spriteData.width * spriteData.scale);
	}

	if (spriteData.isVerticalFlipped) {
		scaling.y *= -1;
		spriteCenter.y -= (float) (spriteData.height * spriteData.scale);
		translate.y += (float) (spriteData.height *spriteData.scale);
	}

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,
		NULL,
		0.0f,
		&scaling,
		&spriteCenter,
		(float) (spriteData.angle),
		&translate
	);

	p_sprite->SetTransform(&matrix);
	p_sprite->Draw(spriteData.texture, &spriteData.rectangle, NULL, NULL, color);
}