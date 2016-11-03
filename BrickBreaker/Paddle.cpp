#include "Paddle.h"

Paddle::Paddle() : GameObject() {
	initialize();
}

Paddle::Paddle(Game* pGame, TextureManager* pTextureManager) {
	initialize();
	GameObject::initialize(
		pGame,
		_imageWidth,
		_imageHeight,
		_numberOfColumns,
		pTextureManager
	);
}

void Paddle::initialize() {
	_imageWidth = 128;
	_imageHeight = 26;
	_spriteData.width = _imageWidth;
	_spriteData.height = _imageHeight;
	_spriteData.x = (GAME_WIDTH * 0.5f) - (this->getWidth() * 0.5f);
	_spriteData.y = (GAME_HEIGHT - this->getHeight() - 15.0f);
	_spriteData.rectangle.bottom = _imageHeight;
	_spriteData.rectangle.right = _imageWidth;
	_frameDelay = 0.0f;
	_startFrame = 0;
	_endFrame = 0;
	_currentFrame = _startFrame;
	_numberOfColumns = 0;
	_edge.top = -_imageHeight / 2;
	_edge.bottom = _imageHeight / 2;
	_edge.left = -_imageWidth / 2;
	_edge.right = _imageWidth / 2;
	_collisionType = CollisionType::RotatedBox;
}

void Paddle::update(float frameTime) {
	GameObject::update(frameTime);

	if (p_input->isGamepadButtonPressed(0, GAMEPAD_DPAD_LEFT) ||
		p_input->isGamepadButtonPressed(0, GAMEPAD_DPAD_RIGHT) ||
		p_input->isKeyDown(MOVE_PADDLE_LEFT) ||
		p_input->isKeyDown(MOVE_PADDLE_RIGHT)) {
		doDpadMovement(frameTime);
	}

	if (p_input->getGamepadThumbX(0, GAMEPAD_LEFT_THUMB) != 0) {
		doThumbstickMovement(frameTime);
	}

	float xPos = this->getX();
	if (xPos < 0) {
		this->setX(0);
	}
	else if (xPos > GAME_WIDTH) {
		this->setX(GAME_WIDTH);
	}
}

void Paddle::doDpadMovement(float frameTime) {
	if (p_input->isGamepadButtonPressed(0, GAMEPAD_DPAD_LEFT) ||
		p_input->isKeyDown(MOVE_PADDLE_LEFT)) {

		if (this->getX() > 0) {
			this->setX(this->getX() - (frameTime * MAX_PADDLE_SPEED));
		}
	}

	if (p_input->isGamepadButtonPressed(0, GAMEPAD_DPAD_RIGHT) ||
		p_input->isKeyDown(MOVE_PADDLE_RIGHT)) {

		if (this->getWidth() + this->getX() < GAME_WIDTH) {
			this->setX(this->getX() + (frameTime * MAX_PADDLE_SPEED));
		}
	}
}

void Paddle::doThumbstickMovement(float frameTime) {
	float thumbX = (float)p_input->getGamepadThumbX(0, GAMEPAD_LEFT_THUMB);
	float distanceFromCenterX = p_input->getThumbstickDistanceFromCenter(thumbX);

	bool isPaddleMovingLeft = thumbX < 0;
	thumbX = MAX_PADDLE_SPEED * distanceFromCenterX;

	if (thumbX != 0) {
		if (!isPaddleMovingLeft) {
			if (this->getWidth() + this->getX() < GAME_WIDTH) {
				this->setX(this->getX() + (frameTime * thumbX));
			}
		}
		else {
			if (this->getX() > 0) {
				this->setX(this->getX() - (frameTime * thumbX));
			}
		}
	}
}