#include "BrickBreaker.h"

using namespace std;

BrickBreaker::BrickBreaker() {
	_currentLayout = new Brick*[8]{ 0 };
}

BrickBreaker::~BrickBreaker() {
	destructCurrentLayout();
}

void BrickBreaker::initialize(HWND hwnd) {
	Game::initialize(hwnd);
	_background = getBackground();

	loadStagesFromFile();
	p_currentStage = &_stages.at(0);
	p_currentLevel = p_currentStage->getLevel(1);
	_currentLevelId = p_currentLevel->getId();
	loadLevel();
	return;
}

void BrickBreaker::update() {
	if (p_input->wasKeyPressed(NEXT_MAP)) {
		p_currentLevel = p_currentStage->getLevel(++_currentLevelId);

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

	vector<GameObject>::iterator iter = _background.begin();
	while (iter != _background.end()) {
		(*iter).draw();
		++iter;
	}

	if (_currentLayout) {
		renderLevel();
	}

	p_graphics->spriteEnd();
	return;
}

void BrickBreaker::calculateCollisions() {
	return;
}

void BrickBreaker::releaseAll() {
	vector<GameObject>::iterator iter = _background.begin();
	while (iter != _background.end()) {
		(*iter).getTextureManager().onLostDevice();
		++iter;
	}
	Game::releaseAll();

	return;
}

void BrickBreaker::resetAll() {
	vector<GameObject>::iterator iter = _background.begin();
	while (iter != _background.end()) {
		(*iter).getTextureManager().onResetDevice();
		++iter;
	}
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
	_currentLayout = new Brick*[8];

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
	if (!_currentLayout) {
		return;
	}

	for (int i = 0; i < 8; i++) {
		safeDeleteArray(_currentLayout[i]);
	}
	safeDeleteArray(_currentLayout);
}

void BrickBreaker::showWinScreen() {
	destructCurrentLayout();
}

vector<GameObject> BrickBreaker::getBackground() {
	vector<GameObject> background;
	GameObject nebula = GameObject(p_graphics, 0, 0, 0, NEBULA_IMAGE);
	GameObject planet = GameObject(p_graphics, 0, 0, 0, PLANET_IMAGE);

	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	background.push_back(nebula);
	background.push_back(planet);
	return background;
}