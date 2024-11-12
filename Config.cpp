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
#include <iostream>

ConfigLoader::ConfigLoader(std::string filename) {
	this->FILE = filename;
}


// Helper function to see if the line 
// is a comment
bool shouldContinue(std::string in) {
	return (in[0] == ';');
}

int ConfigLoader::getInt(std::string key, int defaultValue) {
	std::fstream _file(this->FILE, std::fstream::in | std::fstream::out);
	std::string line;
	while (std::getline(_file, line)) {
		if (shouldContinue(line)) continue;
		if (line.find(key) != std::string::npos) {
			_file.close();
			return std::stoi(line.substr(line.find("=") + 1));
		}
	}
	_file << "\n" << key << "=" << defaultValue;
	_file.close();
	return defaultValue;
}

std::string ConfigLoader::getString(std::string key, const std::string& defaultValue) {
	std::fstream _file(this->FILE, std::fstream::in | std::fstream::out);
	std::string line;
	while (std::getline(_file, line)) {
		if (shouldContinue(line)) continue;
		if (line.find(key) != std::string::npos) {
			_file.close();
			return line.substr(line.find("=") + 1);
		}
	}

	_file.close();
	_file.open(this->FILE, std::fstream::app);
	_file << key << "=" << defaultValue;
	_file.close();

	return defaultValue;
}

bool ConfigLoader::getBool(std::string key, bool defaultValue) {
	std::fstream _file(this->FILE, std::fstream::in | std::fstream::out);
	std::string line;
	while (std::getline(_file, line)) {
		if (shouldContinue(line)) continue;
		if (line.find(key) != std::string::npos) {
			_file.close();
			return line.substr(line.find("=") + 1) == "true";
		}
	}

	_file.close();
	_file.open(this->FILE, std::fstream::app);
	_file << key << "=" << defaultValue;
	_file.close();
	return defaultValue;
}

float ConfigLoader::getFloat(std::string key, float defaultValue) {
	std::fstream _file(this->FILE, std::fstream::in | std::fstream::out);
	std::string line;
	while (std::getline(_file, line)) {
		if (shouldContinue(line)) continue;
		if (line.find(key) != std::string::npos) {
			_file.close();
			return std::stof(line.substr(line.find("=") + 1));
		}
	}

	_file.close();
	_file.open(this->FILE, std::fstream::app);
	_file << "\n" << key << "=" << defaultValue;
	_file.close();

	return defaultValue;
}

void ConfigLoader::setFloat(std::string key, float value) {
	// TODO: IMPLEMENT ME
}

ConfigLoader::~ConfigLoader() {
	// Destructor
	// Nothing to do here
}