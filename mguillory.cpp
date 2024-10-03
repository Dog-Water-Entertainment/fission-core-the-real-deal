// Michael Guillory
// 10/03/2024
// 
//
// The map file should have the following format:
//  e => empty space
//  b => block
//
#include "mguillory.h"
#include <fstream>
#include <iostream>

MapLoader::MapLoader()
{
    mapFileName = "";
}

MapLoader::MapLoader(const char* filename)
{
    mapFileName = filename;
    LoadMapFile(filename);
}

MapLoader::~MapLoader()
{
    mapFileName = "";
}

void MapLoader::LoadMapFile(const char* filename)
{
    mapFileName = filename;
    ifstream mapFile;
    mapFile.open(filename);
}