#pragma once
#ifndef _STAGE_H
#define _STAGE_H

#include <vector>
#include "Level.h"

class Stage {
private:
	int _id;
	int _levelCount;
	std::vector<Level> _levels;

public:
	Stage();
	Stage(const Stage& obj);
	~Stage();

	Stage& operator=(const Stage& rightObj);

	void setId(int number) { _id = number; }
	int getId() { return _id; }
	int getLevelCount() { return _levelCount; }
	void addLevel(Level level) { _levels.push_back(level); _levelCount++; }
	Level* getLevel(int levelId);
	std::vector<Level> getLevels() { return _levels; }
};

#endif