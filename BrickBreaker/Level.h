#pragma once
#ifndef _LEVEL_H
#define _LEVEL_H

#include <string>

//TODO(Logan) -> Refactor the names of brick layout
class Level {
private:
	std::string _name;
	int _number;
	std::string _brickLayout;

public:
	Level();
	virtual ~Level();
	void setName(std::string newName) { _name = newName; }
	void setNumber(int newNumber) { _number = newNumber; }
	void setBrickLayout(std::string newContent) { _brickLayout = newContent; }
	std::string getName() { return _name; }
	int getNumber() { return _number; }
	std::string getBrickLayout() { return _brickLayout; }

};

#endif
