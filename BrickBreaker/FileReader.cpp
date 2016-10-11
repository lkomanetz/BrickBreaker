#include "FileReader.h"

using namespace std;

FileReader::FileReader() :
	_fileLocation("")
{ }

FileReader::FileReader(string fileLocation) : 
	_fileLocation(fileLocation) {

	loadContents();
}

void FileReader::loadContents() {
	if (_fileLocation.empty()) {
		return;
	}

	ifstream iStream(_fileLocation);
	if (!iStream) {
		return;
	}

	string line;
	while (getline(iStream, line)) {
		_fileContents.push_back(line);
	}
}