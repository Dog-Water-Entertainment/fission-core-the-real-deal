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
 *             MATH SECTION
 *
 *
 *
 *
 * ========================================
 */

Vec2::Vec2()
{
    x = 0;
    y = 0;
}

Vec2::Vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vec2::~Vec2()
{
    x = 0;
    y = 0;
}

Vec3::Vec3()
{
    x = 0;
    y = 0;
    z = 0;
}

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3::~Vec3()
{
    x = 0;
    y = 0;
    z = 0;
}


// NormalizedCoord
NormalizedCoord::NormalizedCoord()
{
    coord = Vec2(0, 0);
}

NormalizedCoord::NormalizedCoord(Vec2 coord)
{
    this->coord = coord;
}

NormalizedCoord::NormalizedCoord(float x, float y)
{
    coord = Vec2(x, y);
}

Vec2 NormalizedCoord::getWorldCoord()
{
    // TODO: Return (x * xres/2) + xres/2, (y * yres/2) + yres/2
    return Vec2((coord.x * worldSpace.x / 2) + worldSpace.x / 2,
                (coord.y * worldSpace.y / 2) + worldSpace.y / 2);
}

void NormalizedCoord::setWorldSpace(float x, float y)
{
    worldSpace = Vec2(x, y);
}

void NormalizedCoord::setWorldSpace(Vec2 space)
{
    worldSpace = Vec2((space.x * 2) + 2, (space.y * 2) + 2);
}

Vec2 NormalizedCoord::getCoord()
{
    return coord;
}

void NormalizedCoord::setCoord(Vec2 coord)
{
    this->coord = coord;
}

void NormalizedCoord::setCoord(float x, float y)
{
    coord = Vec2(x, y);
}

NormalizedCoord::~NormalizedCoord()
{
    coord = Vec2(0, 0);
}



// ScreenCoord
ScreenCoord::ScreenCoord(float x, float y)
{
    coord = Vec2(x, y);
}

ScreenCoord::ScreenCoord(Vec2 coord)
{
    this->coord = coord;
}

void ScreenCoord::setWorldSpace(float x, float y)
{
    worldSpace = Vec2(x, y);
}

void ScreenCoord::setWorldSpace(Vec2 space)
{
    worldSpace = space;
}

ScreenCoord::~ScreenCoord()
{
    coord = Vec2(0, 0);
    worldSpace = Vec2(0, 0);
}



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
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            map[i][j] = new EmptyTile();
        }
    }
    center = NormalizedCoord(0, 0).getCoord();
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
    while (mapFile >> line) {
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
    // I would be drawing a buffer around the screen that is 2 tiles larger
    // than the screen size so 20x16 tiles would be rendered
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