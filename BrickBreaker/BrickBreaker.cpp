#include "BrickBreaker.h"

using namespace std;

BrickBreaker::BrickBreaker() {
	_currentLayout = new Brick*[11]{ 0 };
}

BrickBreaker::~BrickBreaker() {
	destructCurrentLayout();
	releaseAll();
}

void BrickBreaker::initialize(HWND hwnd) {
	_currentStageId = 1;
	Game::initialize(hwnd);
	_gameTextures = TextureManager(p_graphics, BRICK_IMAGE);
	_nebulaTexture = TextureManager(p_graphics, NEBULA_IMAGE);
	_planetTexture = TextureManager(p_graphics, PLANET_IMAGE);
	_background = getBackground();

	_gameBall = Ball(p_graphics, &_gameTextures);
	_gameBall.setFrames(3, 3);
	_gameBall.setCurrentFrame(3);
	_gameBall.setX(GAME_WIDTH * 0.5f - _gameBall.getWidth() * 0.5f);
	_gameBall.setY(GAME_HEIGHT - 30.0f);

	_paddle = Paddle(p_graphics, &_gameTextures);
	_paddle.setFrames(4, 4);
	_paddle.setCurrentFrame(4);
	_paddle.setX(_gameBall.getX());
	_paddle.setY(_gameBall.getY() + 15.0f);

	loadStagesFromFile();
	p_currentStage = getStage(_currentStageId);
	p_currentLevel = p_currentStage->getLevel(1);
	_currentLevelId = p_currentLevel->getId();
	loadLevel();
	return;
}

void BrickBreaker::update() {
	if (_currentLayout != NULL) {
		for (UINT i = 0; i < 11; i++) {
			for (UINT j = 0; j < 8; j++) {
				_currentLayout[i][j].update(_frameTime);
			}
		}
	}

	if (p_input->wasKeyPressed(NEXT_MAP)) {
		p_currentLevel = getNextLevel();

		if (!p_currentLevel) {
			showWinScreen();
			return;
		}

		loadLevel();
	}

}

void BrickBreaker::performAi() {
	return;
}

void BrickBreaker::render() {
	p_graphics->spriteBegin();

	vector<Image>::iterator iter = _background.begin();
	while (iter != _background.end()) {
		(*iter).draw();
		++iter;
	}

	if (_currentLayout) {
		renderLayout();
	}

	_gameBall.draw();
	_paddle.draw();

	p_graphics->spriteEnd();
	return;
}

void BrickBreaker::calculateCollisions() {
	return;
}

void BrickBreaker::releaseAll() {
	vector<Image>::iterator iter = _background.begin();
	while (iter != _background.end()) {
		(*iter).releaseTextureManager();
		++iter;
	}
	_gameTextures.onLostDevice();
	Game::releaseAll();

	return;
}

void BrickBreaker::resetAll() {
	vector<Image>::iterator iter = _background.begin();
	while (iter != _background.end()) {
		(*iter).resetTextureManager();
		++iter;
	}
	_gameTextures.onResetDevice();
	Game::resetAll();

	return;
}

void BrickBreaker::loadStagesFromFile() {
	UINT stageNumber = 1;
	std::string fileName = "stages/" + std::to_string(stageNumber) + ".txt";
	FileReader fReader(fileName);
	vector<string> fileContent = fReader.getContents();

	while (fReader.getContents().size() > 0) {
		vector<string> fileContent = fReader.getContents();
		Level level;
		Stage stage;
		stage.setId(stageNumber);
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

		fileName = "stages/" + std::to_string(++stageNumber) + ".txt";
		fReader.setLocation(fileName);
	}
}

void BrickBreaker::loadLevel() {
	destructCurrentLayout();
	_currentLayout = new Brick*[11];

	string levelContent = p_currentLevel->getLayoutString();
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

		Brick brick = Brick(p_graphics, &_gameTextures);

		BrickType type = static_cast<BrickType>(levelContent[i] - '0');
		brick.setType(type);

		switch (type) {
			case BrickType::Indestructible:
				brick.setFrames(0, 0);
				brick.setCurrentFrame(0);
				brick.setHealth(0);
				break;
			case BrickType::Normal:
				brick.setFrames(1, 1);
				brick.setCurrentFrame(1);
				brick.setHealth(1);
				break;
			case BrickType::TwoHit:
				brick.setFrames(2, 2);
				brick.setCurrentFrame(2);
				brick.setHealth(2);
				break;
		}
		
		_currentLayout[brickRow][brickCol] = brick;
		brickCol++;
	}
}

Stage* BrickBreaker::getStage(int stageNumber) {
	vector<Stage>::iterator iter = _stages.begin();
	Stage* returnValue = NULL;

	while (iter != _stages.end()) {
		if ((*iter).getId() == stageNumber) {
			returnValue = &(*iter);
			break;
		}

		++iter;
	}

	return returnValue;
}

void BrickBreaker::renderLayout() {
	float startX = GAME_WIDTH * 0.10;
	float brickWidth = _currentLayout[0][0].getWidth();
	float brickHeight = _currentLayout[0][0].getHeight();
	float brickX = startX;
	float brickY = 0.0f;
	for (UINT i = 0; i < 11; i++) {
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
	if (!_currentLayout) {
		return;
	}

	for (int i = 0; i < 11; i++) {
		safeDeleteArray(_currentLayout[i]);
	}
	safeDeleteArray(_currentLayout);
}

void BrickBreaker::showWinScreen() {
	destructCurrentLayout();
}

vector<Image> BrickBreaker::getBackground() {
	vector<Image> background;
	Image nebula = Image(p_graphics, 0, 0, 0, &_nebulaTexture);
	Image planet = Image(p_graphics, 0, 0, 0, &_planetTexture);

	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	background.push_back(nebula);
	background.push_back(planet);
	return background;
}

Level* BrickBreaker::getNextLevel() {
	if (!p_currentStage) {
		return NULL;
	}

	Level* pNextLevel = NULL;
	pNextLevel = p_currentStage->getLevel(++_currentLevelId);

	if (!pNextLevel) {
		p_currentStage = getStage(++_currentStageId);

		if (!p_currentStage) {
			return NULL;
		}

		_currentLevelId = 1;
		pNextLevel = p_currentStage->getLevel(_currentLevelId);
	}

	return pNextLevel;
}