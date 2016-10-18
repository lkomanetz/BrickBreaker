#include "BrickBreaker.h"

using namespace std;

BrickBreaker::BrickBreaker() :
	_currentStage(1),
	_currentLevel(1) {

	_currentLayout = new Brick*[8]{ 0 };
}

BrickBreaker::~BrickBreaker() {
	delete p_brick;
	destructCurrentLayout();
}

/*
The issue is with TextureManager somehow.
*/
void BrickBreaker::initialize(HWND hwnd) {
	Game::initialize(hwnd);
	_nebula = GameObject(p_graphics, 0, 0, 0, NEBULA_IMAGE);
	_planet = GameObject(p_graphics, 0, 0, 0, PLANET_IMAGE);
	p_brick = new Brick(p_graphics);

	_planet.setX(GAME_WIDTH * 0.5f - _planet.getWidth() * 0.5f);
	_planet.setY(GAME_HEIGHT * 0.5f - _planet.getHeight() * 0.5f);

	p_brick->setX(15.0f);
	p_brick->setY(15.0f);
	
	loadStagesFromFile();
	loadLevel();
	return;
}

//TODO(Logan) -> Remove the _currentLevel and _currentStage variables.
void BrickBreaker::update() {
	if (p_input->wasKeyPressed(NEXT_MAP)) {
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

	_nebula.draw();
	_planet.draw();

	renderLevel();

	p_graphics->spriteEnd();
	return;
}

void BrickBreaker::calculateCollisions() {
	return;
}

void BrickBreaker::releaseAll() {
	_nebula.getTextureManager().onLostDevice();
	_planet.getTextureManager().onLostDevice();
	Game::releaseAll();

	return;
}

void BrickBreaker::resetAll() {
	_nebula.getTextureManager().onResetDevice();
	_nebula.getTextureManager().onResetDevice();
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

	Level level;
	Stage stage;
	stage.setId(_currentStage);
	for (auto iter = fileContent.begin(); iter != fileContent.end(); ++iter) {
		int position = (*iter).find("=");
		string propName = (*iter).substr(0, position);
		string propValue = (*iter).substr(++position, (*iter).length());

		if (propName == "Level") {
			level = Level(p_graphics);
			level.setId(stoi(propValue));
		}
		else if (propName == "Name") {
			level.setName(propValue);
		}
		else if (propName == "Content") {
			level.setLayoutString(propValue);
			stage.addLevel(level);
		}
	}
	_stages.push_back(stage);
}

void BrickBreaker::loadLevel() {
	destructCurrentLayout();
	_currentLayout = new Brick*[8];

	Stage currentStage = _stages.at(_currentStage - 1);
	Level currentLevel = currentStage.getLevel(_currentLevel);
	string levelContent = currentLevel.getLayoutString();
	size_t size = levelContent.size();
	UINT brickRow = 0;
	UINT brickCol = 0;
	_currentLayout[brickRow] = new Brick[8];

	for (UINT i = 0; i < size; i++) {
		if (levelContent[i] == ',') {
			brickRow++;
			brickCol = 0;
			_currentLayout[brickRow] = new Brick[8];
			continue;
		}

		Brick brick = Brick(p_graphics);

		BrickType type = static_cast<BrickType>(levelContent[i] - '0');
		brick.setType(type);
		_currentLayout[brickRow][brickCol] = brick;
		brickCol++;
	}
}

Stage* BrickBreaker::getStage(int stageNumber) {
	return NULL;
}

void BrickBreaker::renderLevel() {
	float startX = 20.0f;
	float brickWidth = _currentLayout[0][0].getWidth();
	float brickHeight = _currentLayout[0][0].getHeight();
	float brickX = startX;
	float brickY = 0.0f;
	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			Brick brick = _currentLayout[i][j];
			if (brick.getType() != BrickType::NoBrick) {
				brick.setX(brickX);
				brick.setY(brickY);
				brick.draw();
			}
			brickX += brickWidth;
		}
		brickY += brickHeight;
		brickX = startX;
	}
}

void BrickBreaker::destructCurrentLayout() {
	for (int i = 0; i < 8; i++) {
		safeDeleteArray(_currentLayout[i]);
	}
	safeDeleteArray(_currentLayout);
}