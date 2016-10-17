#include "BrickBreaker.h"

using namespace std;

BrickBreaker::BrickBreaker() :
	_currentStage(1),
	_currentLevel(1) {

}

BrickBreaker::~BrickBreaker() {
	std::vector<GameObject*>::iterator iter;
	for (iter = _background.begin(); iter != _background.end(); iter++) {
		safeDelete((*iter));
	}
}

/*
The issue is with TextureManager somehow.
*/
void BrickBreaker::initialize(HWND hwnd) {
	Game::initialize(hwnd);
	_nebula = GameObject(p_graphics, 0, 0, 0, NEBULA_IMAGE);
	_planet = GameObject(p_graphics, 0, 0, 0, PLANET_IMAGE);

	_planet.setX(GAME_WIDTH * 0.5f - _planet.getWidth() * 0.5f);
	_planet.setY(GAME_HEIGHT * 0.5f - _planet.getHeight() * 0.5f);
	
	//loadStagesFromFile();
	//loadLevel();
	return;
}

//TODO(Logan) -> Remove the _currentLevel and _currentStage variables.
void BrickBreaker::update() {
	if (p_input->wasKeyPressed(NEXT_MAP)) {
	}
	return;
}

void BrickBreaker::performAi() {
	return;
}

void BrickBreaker::render() {
	p_graphics->spriteBegin();

	//std::vector<GameObject*>::iterator iter;
	//for (iter = _background.begin(); iter != _background.end(); iter++) {
	//	(*iter)->draw();
	//}

	_nebula.draw();
	_planet.draw();

	// _brick.draw();

	p_graphics->spriteEnd();
	return;
}

void BrickBreaker::calculateCollisions() {
	return;
}

void BrickBreaker::releaseAll() {
	vector<GameObject*>::iterator iter;
	for (iter = _background.begin(); iter != _background.end(); iter++) {
		(*iter)->getTextureManager().onLostDevice();
	}
	Game::releaseAll();

	return;
}

void BrickBreaker::resetAll() {
	vector<GameObject*>::iterator iter;
	for (iter = _background.begin(); iter != _background.end(); iter++) {
		(*iter)->getTextureManager().onResetDevice();
	}
	Game::resetAll();

	return;
}

void BrickBreaker::loadStagesFromFile() {
	std::string fileName = "stages/" + std::to_string(_currentStage) + ".txt";
	FileReader fReader(fileName);
	vector<string> fileContent = fReader.getContents();

	if (fileContent.size() == 0) {
		return;
	}
}

void BrickBreaker::loadLevel() {
}

Stage* BrickBreaker::getStage(int stageNumber) {
	return NULL;
}

void BrickBreaker::renderLevel() {
	//Brick*** layout = _level->getLayout();
	//float startX = 20.0f;
	//float brickWidth = layout[0][0]->getWidth();
	//float brickHeight = layout[0][0]->getHeight();
	//float brickX = startX;
	//float brickY = 0.0f;
	//for (UINT i = 0; i < 8; i++) {
	//	for (UINT j = 0; j < 8; j++) {
	//		Brick* brick = layout[i][j];
	//		if (brick->getType() != BrickType::NoBrick) {
	//			brick->setX(brickX);
	//			brick->setY(brickY);
	//			brick->draw();
	//		}
	//		brickX += brickWidth;
	//	}
	//	brickY += brickHeight;
	//	brickX = startX;
	//}
}