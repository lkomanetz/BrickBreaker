#include "Stage.h"

using namespace std;

Stage::Stage() :
	_id(0) {
	_levels = vector<Level>();
}

Stage::Stage(const Stage& obj) {
	_id = obj._id;
	_levelCount = obj._levelCount;

	auto iter = obj._levels.begin();
	for (auto iter = obj._levels.begin(); iter != obj._levels.end(); ++iter) {
		Level copiedLevel = Level((*iter));
		_levels.push_back(copiedLevel);
	}
}

Stage& Stage::operator=(const Stage& rightObj) {
	_id = rightObj._id;
	_levelCount = rightObj._levelCount;

	for (auto iter = rightObj._levels.begin(); iter != rightObj._levels.end(); ++iter) {
		Level copiedLevel = Level((*iter));
		_levels.push_back(copiedLevel);
	}
	return *this;
}

Stage::~Stage() {
}

Level* Stage::getLevel(int levelNumber) {
	Level returnValue;

	auto iter = _levels.begin();
	while (iter != _levels.end()) {
		if ((*iter).getNumber() == levelNumber) {
			returnValue = (*iter);
		}
		++iter;
	}
	return &returnValue;
}