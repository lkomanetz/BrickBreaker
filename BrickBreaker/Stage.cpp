#include "Stage.h"

using namespace std;

Stage::Stage() :
	_id(0) {
	_levels = vector<Level*>();
}

Stage::~Stage() {
	auto iter = _levels.begin();
	while (iter != _levels.end()) {
		safeDelete((*iter));
		++iter;
	}
}

Level* Stage::getLevel(int levelNumber) const {
	Level* returnValue = NULL;

	auto iter = _levels.begin();
	while (iter != _levels.end()) {
		if ((*iter)->getNumber() == levelNumber) {
			returnValue = (*iter);
		}
		++iter;
	}
	return returnValue;
}