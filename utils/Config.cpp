// Michael Guillory
// Date: Oct 14, 2024
// Implementation of a 
// config file


/*
*
* Very Innefficent way to load a config file
* If you have a better way to load a config file
* please refactor this code using hashing or some other
*
*/
#include "Config.h"
#include <fstream>

ConfigLoader::ConfigLoader(std::string filename) {
	this->FILE = filename;
}


// Helper function to see if the line 
// is a comment
bool shouldContinue(std::string in) {
	return (in[0] == ';');
}

int ConfigLoader::getInt(std::string key) {
	std::ifstream _file(this->FILE);
	std::string line;
	while (std::getline(_file, line)) {
		if (shouldContinue(line)) continue;
		if (line.find(key) != std::string::npos) {
			_file.close();
			return std::stoi(line.substr(line.find("=") + 1));
		}
	}
	_file.close();
	return -1;
}

std::string ConfigLoader::getString(std::string key) {
	std::ifstream _file(this->FILE);
	std::string line;
	while (std::getline(_file, line)) {
		if (shouldContinue(line)) continue;
		if (line.find(key) != std::string::npos) {
			_file.close();
			return line.substr(line.find("=") + 1);
		}
	}
	_file.close();
	return "";
}

bool ConfigLoader::getBool(std::string key) {
	std::ifstream _file(this->FILE);
	std::string line;
	while (std::getline(_file, line)) {
		if (shouldContinue(line)) continue;
		if (line.find(key) != std::string::npos) {
			_file.close();
			return line.substr(line.find("=") + 1) == "true";
		}
	}
	_file.close();
	return false;
}

float ConfigLoader::getFloat(std::string key) {
	std::ifstream _file(this->FILE);
	std::string line;
	while (std::getline(_file, line)) {
		if (shouldContinue(line)) continue;
		if (line.find(key) != std::string::npos) {
			_file.close();
			return std::stof(line.substr(line.find("=") + 1));
		}
	}
	_file.close();
	return -1.0f;
}

ConfigLoader::~ConfigLoader() {
	// Destructor
	// Nothing to do here
}