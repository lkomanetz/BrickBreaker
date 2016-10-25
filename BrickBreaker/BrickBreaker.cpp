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
	//NOTE(Logan) -> Might be able to have one texture manager with all assets but it may not work.
	_ballTexture = TextureManager(p_graphics, BALL_IMAGE);
	_paddleTexture = TextureManager(p_graphics, PADDLE_IMAGE);
	_brickTextures = TextureManager(p_graphics, BRICK_IMAGE);
	_nebulaTexture = TextureManager(p_graphics, NEBULA_IMAGE);
	_planetTexture = TextureManager(p_graphics, PLANET_IMAGE);
	_background = getBackground();

	_paddle = Paddle(p_graphics, &_paddleTexture);
	_paddle.setX(GAME_WIDTH * 0.5f - _paddle.getWidth() * 0.5f);
	_paddle.setY(GAME_HEIGHT - _paddle.getHeight() - 15.0f);

	_gameBall = Ball(p_graphics, &_ballTexture);
	_gameBall.setX(GAME_WIDTH * 0.5f - _gameBall.getWidth() * 0.5f);
	_gameBall.setY(_paddle.getY() - 15.0f);

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
	_brickTextures.onLostDevice();
	_ballTexture.onLostDevice();
	_paddleTexture.onLostDevice();
	Game::releaseAll();

	return;
}

void BrickBreaker::resetAll() {
	vector<Image>::iterator iter = _background.begin();
	while (iter != _background.end()) {
		(*iter).resetTextureManager();
		++iter;
	}
	_brickTextures.onResetDevice();
	_ballTexture.onResetDevice();
	_paddleTexture.onResetDevice();
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
			size_t position = (*iter).find("=");
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

		Brick brick = Brick(p_graphics, &_brickTextures);

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
	float brickWidth = (float)_currentLayout[0][0].getWidth();
	float brickHeight = (float)_currentLayout[0][0].getHeight();
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
