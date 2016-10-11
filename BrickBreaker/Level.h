#pragma once
#ifndef _LEVEL_H
#define _LEVEL_H

#include <string>

class Level {
private:
	std::string _name;
	int _number;
	std::string _content;

public:
	Level();
	virtual ~Level();
	void setName(std::string newName) { _name = newName; }
	void setNumber(int newNumber) { _number = newNumber; }
	void setContent(std::string newContent) { _content = newContent; }
	std::string getName() { return _name; }
	int getNumber() { return _number; }
	std::string getContent() { return _content; }

};

#endif
