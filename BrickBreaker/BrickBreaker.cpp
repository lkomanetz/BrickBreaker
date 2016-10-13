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

//TODO(Logan) -> Make sure there are bounds checking
void BrickBreaker::update() {
	if (p_input->isKeyDown(NEXT_MAP)) {
		_currentLevel++;
		loadLevel();
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

	float startX = 20.0f;
	float brickWidth = p_currentLayout[0][0]->getWidth();
	float brickHeight = p_currentLayout[0][0]->getHeight();
	float brickX = startX;
	float brickY = 0.0f;
	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			if (p_currentLayout[i][j]->getBrickType() != BrickType::NoBrick) {
				p_currentLayout[i][j]->setX(brickX);
				p_currentLayout[i][j]->setY(brickY);
				p_currentLayout[i][j]->draw();
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
	Stage* currentStage = new Stage();
	vector<Stage*>::iterator iter;
	for (iter = _stages.begin(); iter != _stages.end(); ++iter) {
		if ((*iter)->getNumber() == _currentStage) {
			currentStage = (*iter);
		}
	}

	Level* pLevel = NULL;
	pLevel = currentStage->getLevel(_currentLevel);

	if (!pLevel) {
		return;
	}
	p_currentLayout = pLevel->buildLayout();
}