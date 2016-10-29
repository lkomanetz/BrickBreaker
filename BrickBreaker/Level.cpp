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
}


void Level::construct() {
	_id = 0;
	_layoutString = "";
	_name = "";
	p_graphics = NULL;
}
