#include "Level.h"

using namespace std;

Level::Level() {
	construct();
}

Level::Level(Graphics* pGraphics) {
	construct();
	p_graphics = pGraphics;
}

Level::Level(const Level& obj) {
	_name = obj._name;
	_id = obj._id;
	p_graphics = obj.p_graphics;
	_layoutString = obj._layoutString;
}

Level::~Level() {
}

Level& Level::operator=(const Level& rightObj) {
	_name = rightObj._name;
	_id = rightObj._id;
	p_graphics = rightObj.p_graphics;
	_layoutString = rightObj._layoutString;

	return *this;
}

void Level::setLayoutString(string newContent) {
	_layoutString = newContent;
	buildLayout();
}


void Level::construct() {
	_id = 0;
	_layoutString = "";
	_name = "";
	p_graphics = NULL;
}

void Level::buildLayout() {
	//string levelContent = getLayoutString();
	//size_t size = _layoutString.size();
	//UINT brickRow = 0;
	//UINT brickCol = 0;
	//p_brickLayout = new Brick**[8];
	//p_brickLayout[brickRow] = new Brick*[8];

	//for (UINT i = 0; i < size; i++) {
	//	if (levelContent[i] == ',') {
	//		brickRow++;
	//		brickCol = 0;
	//		p_brickLayout[brickRow] = new Brick*[8];
	//		continue;
	//	}

	//	Brick* brick = new Brick(p_graphics);

	//	BrickType type = static_cast<BrickType>(levelContent[i] - '0');
	//	brick->setType(type);
	//	p_brickLayout[brickRow][brickCol] = brick;
	//	brickCol++;
	//}
}