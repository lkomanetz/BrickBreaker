#include "Level.h"

using namespace std;

Level::Level() {
	construct();
}

Level::Level(Graphics* pGraphics) {
	construct();
	p_graphics = pGraphics;
}

Level::~Level() {
	if (_layout == NULL) {
		return;
	}

	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			safeDelete(_layout[i][j]);
		}
		safeDeleteArray(_layout[i]);
	}
}

void Level::construct() {
	_number = 0;
	_layoutString = "";
	_name = "";
	p_graphics = NULL;
}

Brick*** Level::buildLayout() {
	string levelContent = getLayoutString();
	UINT size = _layoutString.size();
	UINT brickRow = 0;
	UINT brickCol = 0;
	_layout = new Brick**[8];
	_layout[brickRow] = new Brick*[8];

	for (UINT i = 0; i < size; i++) {
		if (levelContent[i] == ',') {
			brickRow++;
			brickCol = 0;
			_layout[brickRow] = new Brick*[8];
			continue;
		}

		Brick* brick = new Brick(p_graphics);

		BrickType type = static_cast<BrickType>(levelContent[i] - '0');
		brick->setBrickType(type);
		_layout[brickRow][brickCol] = brick;
		brickCol++;
	}

	return _layout;
}