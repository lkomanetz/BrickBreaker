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

	deleteCurrentLayout();
	for (vector<Stage*>::iterator it = _stages.begin(); it != _stages.end(); ++it) {
		delete (*it);
	}
}

void BrickBreaker::initialize(HWND hwnd) {
	Game::initialize(hwnd);

	GameObject* _nebula = new GameObject(p_graphics, 0, 0, 0, NEBULA_IMAGE);
	GameObject* _planet = new GameObject(p_graphics, 0, 0, 0, PLANET_IMAGE);

	_planet->setX(GAME_WIDTH * 0.5f - _planet->getWidth() * 0.5f);
	_planet->setY(GAME_HEIGHT * 0.5f - _planet->getHeight() * 0.5f);

	_background.push_back(_nebula);
	_background.push_back(_planet);
	
	loadStagesFromFile();
	loadLevel();
	return;
}

void BrickBreaker::update() {
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

	float startX = 20.0f;
	float brickWidth = _objects[0][0]->getWidth();
	float brickHeight = _objects[0][0]->getHeight();
	float brickX = startX;
	float brickY = 0.0f;
	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			if (_objects[i][j]->getBrickType() != BrickType::NoBrick) {
				_objects[i][j]->setX(brickX);
				_objects[i][j]->setY(brickY);
				_objects[i][j]->draw();
			}
			brickX += brickWidth;
		}
		brickY += brickHeight;
		brickX = startX;
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

	Level level;
	Stage* stage = new Stage();
	stage->setNumber(1);
	vector<string>::iterator iter;
	for (iter = fileContent.begin(); iter != fileContent.end(); ++iter) {
		int position = (*iter).find("=");
		string propertyName = (*iter).substr(0, position);
		string propertyValue = (*iter).substr(++position, (*iter).length());

		if (propertyName == "Level") {
			level.setNumber(stoi(propertyValue));
		}
		else if (propertyName == "Name") {
			level.setName(propertyValue);
		}
		else if (propertyName == "Content") {
			level.setBrickLayout(propertyValue);
		}
		stage->addLevel(level);
	}
	_stages.push_back(stage);
}

void BrickBreaker::loadLevel() {
	deleteCurrentLayout();
	Stage* currentStage = new Stage();
	vector<Stage*>::iterator iter;
	for (iter = _stages.begin(); iter != _stages.end(); ++iter) {
		if ((*iter)->getNumber() == _currentStage) {
			currentStage = (*iter);
		}
	}

	Level* level = NULL;
	level = currentStage->getLevel(_currentLevel);

	//TODO(Logan) -> abstract this code into the Level class to pass back a Brick*** type.
	string levelContent = level->getBrickLayout();
	UINT size = levelContent.size();
	UINT brickRow = 0;
	UINT brickCol = 0;
	_objects = new Brick**[8];
	_objects[brickRow] = new Brick*[8];

	for (UINT i = 0; i < size; i++) {
		if (levelContent[i] == ',') {
			brickRow++;
			brickCol = 0;
			_objects[brickRow] = new Brick*[8];
			continue;
		}

		Brick* brick = new Brick(p_graphics);

		BrickType type = static_cast<BrickType>(levelContent[i] - '0');
		brick->setBrickType(type);
		_objects[brickRow][brickCol] = brick;
		brickCol++;
	}

}

void BrickBreaker::deleteCurrentLayout() {
	if (_objects == NULL) {
		return;
	}

	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			safeDelete(_objects[i][j]);
		}
		safeDeleteArray(_objects[i]);
	}
}