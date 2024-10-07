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
#include <GL/glx.h>
#include <cmath>


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

Vec2 Vec2::operator+(float rhs)
{
    return Vec2(x + rhs, y + rhs);
}

Vec2 Vec2::operator*(float rhs)
{
    return Vec2(x * rhs, y * rhs);
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

Vec3 Vec3::operator+(float rhs)
{
    return Vec3(x + rhs, y + rhs, z + rhs);
}

Vec3 Vec3::operator*(float rhs)
{
    return Vec3(x * rhs, y * rhs, z * rhs);
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

Vec2 ScreenCoord::getWorldCoord()
{
    return Vec2(coord.x + worldSpace.x / 2,
                coord.y + worldSpace.y / 2);
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
    center = Vec2(25, 25);
    playerPos = Vec2(0, 0);
}

MapLoader::MapLoader(const char* filename)
{
    mapFileName = filename;
    center = Vec2(25, 25);
    playerPos = Vec2(0, 0);
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

void MapLoader::setFileName(const char* filename)
{
    mapFileName = filename;
}

void MapLoader::LoadMapFile()
{

    // TODO: This function doesn't work so I need to fix it
    if (mapFileName == "") {
        std::cerr << "Error: No map file specified" << std::endl;
        return;
    }

    int tilex = 50;
    int tiley = 50;


    std::ifstream mapFile;
    mapFile.open(mapFileName.c_str());

    if (!mapFile.is_open()) {
        std::cerr << "Error: Could not open file " << mapFileName << std::endl;
        return;
    }

    int i = 0, j = 0;

    std::string line;
    while (mapFile >> line) {
        if(i < tilex && j < tiley) {
            if (line == "e") {
                map[i][j] = new EmptyTile();
                map[i][j]->id = 'e';
            } else if (line == "b") {
                map[i][j] = new BlockTile();
                map[i][j]->id = 'b';
            } else {
                std::cerr << "Error: Invalid character in map file " 
                         << mapFileName << std::endl;
                return;
            }
            i++;
        } else if(i == tilex && j < tiley) {
            i = 0;
            j++;
        } else if(i == tilex && j == tiley) {
            std::cout << "Error: Map file " << mapFileName << " is too large" 
                      << std::endl;
            break;
        } else {
            std::cerr << "Error: Unknown error reading file " << mapFileName 
                      << std::endl;
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

    Vec2 tileCenter = Vec2(floor(playerPos.x + center.x), floor(playerPos.y + center.y));
    int startingTileY = tileCenter.x - 8;
    int startingTileX = tileCenter.y - 10;

    Vec2 offset = Vec2(playerPos.x - floor(playerPos.x), playerPos.y - floor(playerPos.y));

    Vec2 screenCenter = Vec2(400, 300);

    //Vec2 currentRenderPos = Vec2(screenCenter.x - 500 - offset.x * 50,
    //                            screenCenter.y + 400 - offset.y * 50);
                                
    Vec2 currentRenderPos = Vec2(-100 - offset.x * 50,
                                700 - offset.y * 50);

    for (int i = startingTileY; i < startingTileY + 16; i++) {
        for (int j = startingTileX; j < startingTileX + 20; j++) {
            if (i >= 0 && i < 50 && j >= 0 && j < 50) {
                map[i][j]->render(currentRenderPos);
            }
            currentRenderPos.x += 50;
        }
        currentRenderPos.x = -100 - offset.x * 50;
        currentRenderPos.y -= 50;
    }
}

void MapLoader::setPlayerPos(float x, float y)
{
    playerPos = Vec2(x, y);
}

void MapLoader::setPlayerPos(Vec2 pos)
{
    playerPos = pos;
}

Vec2 MapLoader::getPlayerPos()
{
    return playerPos;
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

void BlockTile::render(Vec2 &pos)
{
    // TODO: Add textures
    //glEnable(GL_TEXTURE_2D);
	//glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.0f);
	//glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
	glColor4f(1.0f, 0.2f, 0.0f, 0.8f);
	glBegin(GL_QUADS);
		//glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		//glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 250);
		//glTexCoord2f(1.0f, 0.0f); glVertex2i(250, 250);
		//glTexCoord2f(1.0f, 1.0f); glVertex2i(250, 0);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x, pos.y + 50);
        glVertex2f(pos.x + 50, pos.y + 50);
        glVertex2f(pos.x + 50, pos.y);
	glEnd();
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_ALPHA_TEST);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void EmptyTile::render(Vec2 &pos)
{
    // TODO: Add textures
    //glEnable(GL_TEXTURE_2D);
	//glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.0f);
	//glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
	glColor4f(1.0f, 0.8f, 1.0f, 0.8f);
	glBegin(GL_QUADS);
		//glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		//glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 250);
		//glTexCoord2f(1.0f, 0.0f); glVertex2i(250, 250);
		//glTexCoord2f(1.0f, 1.0f); glVertex2i(250, 0);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x, pos.y + 50);
        glVertex2f(pos.x + 50, pos.y + 50);
        glVertex2f(pos.x + 50, pos.y);
	glEnd();
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_ALPHA_TEST);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
