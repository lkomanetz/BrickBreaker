#pragma once
#ifndef _FILEREADER_H
#define _FILEREADER_H

#include <vector>
#include <fstream>
#include <string>

class FileReader {
private:
	std::string _fileLocation;
	std::vector<std::string> _fileContents;

	void loadContents();
public:
	FileReader();
	FileReader(std::string fileLocation);
	void setLocation(std::string fileLocation);
	std::string getLocation() { return _fileLocation; }
	std::vector<std::string> getContents() { return _fileContents; }
};

#endif