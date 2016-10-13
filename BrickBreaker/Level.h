#pragma once
#ifndef _LEVEL_H
#define _LEVEL_H

#include <string>
#include "Brick.h"
#include "Graphics.h"

class Level {
private:
	std::string _name;
	int _number;
	std::string _layoutString;
	Brick*** p_brickLayout;
	Graphics* p_graphics;

	void construct();
public:
	Level();
	Level(Graphics* pGraphics);
	virtual ~Level();
	void setName(std::string newName) { _name = newName; }
	void setNumber(int newNumber) { _number = newNumber; }
	void setLayoutString(std::string newContent) { _layoutString = newContent; }
	std::string getName() { return _name; }
	int getNumber() { return _number; }
	std::string getLayoutString() { return _layoutString; }
	Brick*** buildLayout();

};

#endif
