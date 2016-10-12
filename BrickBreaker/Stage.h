#pragma once
#ifndef _STAGE_H
#define _STAGE_H

#include <vector>
#include "Level.h"

class Stage {
private:
	int _number;
	std::vector<Level>* _levels;

public:
	Stage();
	~Stage();
	void setNumber(int number) { _number = number; }
	int getNumber() { return _number; }
	void addLevel(Level level) { _levels->push_back(level); }
	Level* getLevel(int levelNumber) const;
	std::vector<Level>* getLevels() const { return _levels; }
};

#endif