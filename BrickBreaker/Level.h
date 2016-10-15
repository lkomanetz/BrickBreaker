#pragma once
#ifndef _LEVEL_H
#define _LEVEL_H

#include <string>
#include "Brick.h"
#include "Graphics.h"

class Level {
private:
	std::string _name;
	int _id;
	std::string _layoutString;
	Brick*** p_brickLayout;
	Graphics* p_graphics;

	void construct();
	void destructLayout();

public:
	Level();
	Level(Graphics* pGraphics);
	virtual ~Level();
	void setName(std::string newName) { _name = newName; }
	void setId(int newId) { _id = newId; }
	void setLayoutString(std::string newContent);
	Brick*** getLayout() { return p_brickLayout; }
	std::string getName() { return _name; }
	int getNumber() { return _id; }
	std::string getLayoutString() { return _layoutString; }
	void buildLayout();

};

#endif
