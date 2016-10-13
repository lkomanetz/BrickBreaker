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
	if (p_brickLayout == NULL) {
		return;
	}

	for (UINT i = 0; i < 8; i++) {
		for (UINT j = 0; j < 8; j++) {
			safeDelete(p_brickLayout[i][j]);
		}
		safeDeleteArray(p_brickLayout[i]);
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
	p_brickLayout = new Brick**[8];
	p_brickLayout[brickRow] = new Brick*[8];

	for (UINT i = 0; i < size; i++) {
		if (levelContent[i] == ',') {
			brickRow++;
			brickCol = 0;
			p_brickLayout[brickRow] = new Brick*[8];
			continue;
		}

		Brick* brick = new Brick(p_graphics);

		BrickType type = static_cast<BrickType>(levelContent[i] - '0');
		brick->setType(type);
		p_brickLayout[brickRow][brickCol] = brick;
		brickCol++;
	}

	return p_brickLayout;
}