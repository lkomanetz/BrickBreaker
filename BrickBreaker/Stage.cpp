#include "Stage.h"

using namespace std;

Stage::Stage() :
	_id(0) {
	_levels = vector<Level*>();
}

Stage::~Stage() {
	safeDelete(_levels);
}

Level* Stage::getLevel(int levelNumber) const {
	Level* returnValue = NULL;

	vector<Level*>::iterator iter;
	for (iter = _levels.begin(); iter != _levels.end(); ++iter) {
		if ((*iter)->getNumber() == levelNumber) {
			returnValue = (*iter);
		}
	}

	return returnValue;
}