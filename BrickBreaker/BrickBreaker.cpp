#include "BrickBreaker.h"

using namespace std;

BrickBreaker::BrickBreaker() :
	_currentStage(1),
	_currentLevel(1) {

}

BrickBreaker::~BrickBreaker() {
	std::vector<GameObject*>::iterator iter;
	for (iter = _background.begin(); iter != _background.end(); iter++) {
		delete (*iter);
	}

	for (vector<Stage*>::iterator it = _stages.begin(); it != _stages.end(); ++it) {
		delete (*it);
	}
}

void BrickBreaker::initialize(HWND hwnd) {
	Game::initialize(hwnd);

	GameObject* pNebula = new GameObject(p_graphics, 0, 0, 0, NEBULA_IMAGE);
	GameObject* pPlanet = new GameObject(p_graphics, 0, 0, 0, PLANET_IMAGE);

	pPlanet->setX(GAME_WIDTH * 0.5f - pPlanet->getWidth() * 0.5f);
	pPlanet->setY(GAME_HEIGHT * 0.5f - pPlanet->getHeight() * 0.5f);

	_background.push_back(pNebula);
	_background.push_back(pPlanet);
	
	loadStagesFromFile();
	loadLevel();
	return;
}

//TODO(Logan) -> Refactor the code that changes levels and stages along with drawing it on the screen.
void BrickBreaker::update() {
	if (p_input->wasKeyPressed(NEXT_MAP)) {
		Stage* currentStage = getStage(_currentStage);
		if (!currentStage) {
			return;
		}

		int levelCount = currentStage->getLevelCount();
		if (_currentLevel == levelCount) {
			_currentLevel = 0;
			_currentStage++;
		}

		if (_currentLevel < levelCount) {
			_currentLevel++;
		}

		if (_currentLevel <= levelCount) {
			loadLevel();
		}
	}
	return;
}

void BrickBreaker::performAi() {
	return;
}

void BrickBreaker::render() {
	p_graphics->spriteBegin();

	std::vector<GameObject*>::iterator iter;
	for (iter = _background.begin(); iter != _background.end(); iter++) {
		(*iter)->draw();
	}

	if (p_currentLayout) {
		drawBrickLayout();
	}

	p_graphics->spriteEnd();

	return;
}

void BrickBreaker::calculateCollisions() {
	return;
}

void BrickBreaker::releaseAll() {
	vector<GameObject*>::iterator iter;
	for (iter = _background.begin(); iter != _background.end(); iter++) {
		(*iter)->getTextureManager()->onLostDevice();
	}
	Game::releaseAll();

	return;
}

void BrickBreaker::resetAll() {
	vector<GameObject*>::iterator iter;
	for (iter = _background.begin(); iter != _background.end(); iter++) {
		(*iter)->getTextureManager()->onResetDevice();
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

	Level* level = NULL;
	Stage* stage = new Stage();
	stage->setNumber(1);
	vector<string>::iterator iter;
	for (iter = fileContent.begin(); iter != fileContent.end(); ++iter) {
		int position = (*iter).find("=");
		string propertyName = (*iter).substr(0, position);
		string propertyValue = (*iter).substr(++position, (*iter).length());

		if (propertyName == "Level") {
			level = new Level(p_graphics);
			level->setNumber(stoi(propertyValue));
		}
		else if (propertyName == "Name") {
			level->setName(propertyValue);
		}
		else if (propertyName == "Content") {
			level->setLayoutString(propertyValue);
			stage->addLevel(*level);
			delete level;
		}
	}
	_stages.push_back(stage);
}

void BrickBreaker::loadLevel() {
	Stage* currentStage = getStage(_currentStage);
	if (!currentStage) {
		p_currentLayout = NULL;
		return;
	}

	Level* pLevel = NULL;
	pLevel = currentStage->getLevel(_currentLevel);

	if (!pLevel) {
		return;
	}
	p_currentLayout = pLevel->buildLayout();
}

Stage* BrickBreaker::getStage(int stageNumber) {
	Stage* currentStage = NULL;
	vector<Stage*>::iterator iter;
	for (iter = _stages.begin(); iter != _stages.end(); ++iter) {
		if ((*iter)->getNumber() == stageNumber) {
			currentStage = (*iter);
			break;
		}
	}

	return currentStage;
}

void BrickBreaker::drawBrickLayout() {
	float startX = 20.0f;
	float brickWidth = p_currentLayout[0][0]->getWidth();
	float brickHeight = p_currentLayout[0][0]->getHeight();
	float brickX = startX;
	float brickY = 0.0f;
	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			Brick* brick = p_currentLayout[i][j];
			if (brick->getType() != BrickType::NoBrick) {
				brick->setX(brickX);
				brick->setY(brickY);
				brick->draw();
			}
			brickX += brickWidth;
		}
		brickY += brickHeight;
		brickX = startX;
	}
}