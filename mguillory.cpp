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


/*
 * ========================================
 *
 *
 *
 *             MAP SECTION
 *
 *
 *
 *
 * ========================================
 */

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
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 12; j++) {
            delete map[i][j];
        }
    }
}

void MapLoader::LoadMapFile(const char* filename)
{
    int tilex = 50;
    int tiley = 50;

    mapFileName = filename;
    std::ifstream mapFile;
    mapFile.open(filename);

    if (!mapFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    int i = 0, j = 0;

    std::string line;
    while (std::cin >> line) {
        if(i < tilex && j < tiley) {
            if (line == "e") {
                map[i][j] = new EmptyTile();
            } else if (line == "b") {
                map[i][j] = new BlockTile();
            } else {
                std::cerr << "Error: Invalid character in map file " << filename << std::endl;
                return;
            }
            i++;
        } else if(i == tilex && j < tiley) {
            i = 0;
            j++;
        } else if(i == tilex && j == tiley) {
            std::cout << "Error: Map file " << filename << " is too large" << std::endl;
            break;
        } else {
            std::cerr << "Error: Unknown error reading file " << filename << std::endl;
            break;
        }
    }

    mapFile.close();
}

void MapLoader::render()
{
    // TODO: Implement render
    // Where it only renders the tiles that are visible on the screen
    // For example, if the screen is 800x600 and each tile is 50x50, then
    // Each tile should be 50x50 pixels on the screen and only 16x12 tiles
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 12; j++) {
            map[i][j]->render();
        }
    }
}

/*
 * ========================================
 *
 *
 *
 *            TILE SECTION
 *
 *
 *
 *
 * ========================================
 */

void BlockTile::render()
{
    std::cout << "Block";
}

void EmptyTile::render()
{
    std::cout << "Empty";
}