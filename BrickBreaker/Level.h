#pragma once
#ifndef _LEVEL_H
#define _LEVEL_H

#include <string>
#include "Brick.h"
#include "Engine/Graphics.h"

class Level {
private:
	std::string _name;
	int _id;
	std::string _layoutString;
	Graphics* p_graphics;

	void construct();

public:
	Level();
	Level(Graphics* pGraphics);
	Level(const Level& obj);
	virtual ~Level();

	Level& operator=(const Level& rightObj);

	void setName(std::string newName) { _name = newName; }
	void setId(int newId) { _id = newId; }
	void setLayoutString(std::string newContent);
	std::string getName() { return _name; }
	int getId() const { return _id; }
	std::string getLayoutString() { return _layoutString; }
	void buildLayout();

};

#endif
