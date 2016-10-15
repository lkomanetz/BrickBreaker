#pragma once
#ifndef _STAGE_H
#define _STAGE_H

#include <vector>
#include "Level.h"

class Stage {
private:
	int _id;
	int _levelCount;
	std::vector<Level*> _levels;

public:
	Stage();
	~Stage();
	void setId(int number) { _id = number; }
	int getId() { return _id; }
	int getLevelCount() { return _levelCount; }
	void addLevel(Level* level) { _levels.push_back(level); _levelCount++; }
	Level* getLevel(int levelId) const;
	std::vector<Level*> getLevels() const { return _levels; }
};

#endif