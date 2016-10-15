#include "BrickBreaker.h"

using namespace std;

BrickBreaker::BrickBreaker() :
	_currentStage(1),
	_currentLevel(1),
	_level(NULL) {

}

BrickBreaker::~BrickBreaker() {
	std::vector<GameObject*>::iterator iter;
	for (iter = _background.begin(); iter != _background.end(); iter++) {
		delete (*iter);
	}

	for (vector<Stage*>::iterator it = _stages.begin(); it != _stages.end(); ++it) {
		delete (*it);
	}
	safeDelete(_level);
	safeDelete(_stage);
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
	_stage = getStage(_currentStage);
	loadLevel();
	return;
}

//TODO(Logan) -> Remove the _currentLevel and _currentStage variables.
void BrickBreaker::update() {
	if (p_input->wasKeyPressed(NEXT_MAP)) {
		if (_stage == NULL) {
			safeDelete(_stage);
			return;
		}

		if (_currentLevel == _stage->getLevelCount()) {
			_currentLevel = 0;
			_currentStage++;
			_stage = getStage(_currentStage);
		}

		if (_stage == NULL) {
			_level = NULL;
			safeDelete(_level);
			safeDelete(_stage);
			return;
		}

		int levelCount = _stage->getLevelCount();
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

	if (_level != NULL) {
		renderLevel();
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
	stage->setId(1);
	vector<string>::iterator iter;
	for (iter = fileContent.begin(); iter != fileContent.end(); ++iter) {
		int position = (*iter).find("=");
		string propertyName = (*iter).substr(0, position);
		string propertyValue = (*iter).substr(++position, (*iter).length());

		if (propertyName == "Level") {
			level = new Level(p_graphics);
			level->setId(stoi(propertyValue));
		}
		else if (propertyName == "Name") {
			level->setName(propertyValue);
		}
		else if (propertyName == "Content") {
			level->setLayoutString(propertyValue);
			stage->addLevel(level);
		}
	}
	_stages.push_back(stage);
}

void BrickBreaker::loadLevel() {
	if (_stage == NULL) {
		delete _stage;
		return;
	}

	if (_level == NULL) {
		delete _level;
		_level = _stage->getLevel(1);
	}
	else {
		int levelNumber = _level->getNumber();
		_level = _stage->getLevel(++levelNumber);
	}
}

Stage* BrickBreaker::getStage(int stageNumber) {
	Stage* currentStage = NULL;
	vector<Stage*>::iterator iter;
	for (iter = _stages.begin(); iter != _stages.end(); ++iter) {
		if ((*iter)->getId() == stageNumber) {
			currentStage = (*iter);
			break;
		}
	}

	return currentStage;
}

void BrickBreaker::renderLevel() {
	Brick*** layout = _level->getLayout();
	float startX = 20.0f;
	float brickWidth = layout[0][0]->getWidth();
	float brickHeight = layout[0][0]->getHeight();
	float brickX = startX;
	float brickY = 0.0f;
	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			Brick* brick = layout[i][j];
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