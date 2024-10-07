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
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <GL/glx.h>
#include <cmath>
#include <X11/Xlib.h>
#include <set>
#include "Image.h"

static Image img[1] = {
	"./images/map_textures/grass.jpg"
};

class global 
{
public:
    GLuint grassTexture;
    global() {
        //nothing
    }
} static g;

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
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            map[i][j] = new EmptyTile();
        }
    }
    center = Vec2(25, 25);
    playerPos = Vec2(0, 0);
}

MapLoader::MapLoader(const char *filename)
{
    mapFileName = filename;
    center = Vec2(25, 25);
    playerPos = Vec2(0, 0);
}

MapLoader::~MapLoader()
{
    mapFileName = "";
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            delete map[i][j];
        }
    }
}

void MapLoader::setFileName(const char *filename)
{
    mapFileName = filename;
}

void MapLoader::loadTextures()
{
    glGenTextures(1, &g.grassTexture);


    // For grass
    int w = img[0].width;
	int h = img[0].height;

    glBindTexture(GL_TEXTURE_2D, g.grassTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *grassData = buildAlphaData(&img[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, grassData);
	free(grassData);
}

void MapLoader::LoadMapFile()
{
    if (mapFileName == "")
    {
        std::cerr << "Error: No map file specified" << std::endl;
        return;
    }

    int tilex = 50;
    int tiley = 50;

    std::ifstream mapFile(mapFileName.c_str());

    if (!mapFile.is_open())
    {
        std::cerr << "Error: Could not open file " << mapFileName << std::endl;
        return;
    }

    std::string line;
    int i = 0;
    while (std::getline(mapFile, line) && i < tilex)
    {
        int j = 0;
        for (char &ch : line)
        {
            if (j < tiley)
            {
                if (ch == 'e')
                {
                    map[i][j] = new EmptyTile();
                    map[i][j]->id = 'e';
                }
                else if (ch == 'b')
                {
                    map[i][j] = new BlockTile();
                    map[i][j]->id = 'b';
                }
                else if (ch == ' ')
                {
                    continue;
                }
                else
                {
                    std::cerr << "Error: Invalid character in map file "
                              << mapFileName << std::endl;
                    return;
                }
                j++;
            }
            else
            {
                std::cerr << "Error: Map file " << mapFileName << " is too large"
                          << std::endl;
                break;
            }
        }
        i++;
    }

    // cout  the whole map to the console
    for (int i = 0; i < tilex; i++)
    {
        for (int j = 0; j < tiley; j++)
        {
            std::cout << map[i][j]->id;
        }
        std::cout << std::endl;
    }

    mapFile.close();
}

void MapLoader::render()
{
    // Calculate the center tile based on the player's position
    Vec2 tileCenter = Vec2(floor(playerPos.x + center.x), floor(playerPos.y + center.y));
    int startingTileY = tileCenter.y - 8; // Adjusted for correct axis
    int startingTileX = tileCenter.x - 10; // Adjusted for correct axis

    // Calculate the offset for smooth movement
    Vec2 offset = Vec2(playerPos.x - floor(playerPos.x), playerPos.y - floor(playerPos.y));

    // Calculate the initial render position
    Vec2 currentRenderPos = Vec2(-100 - offset.x * 50, 700 - offset.y * 50);

    // Render only the visible tiles plus a buffer
    for (int i = startingTileY; i < startingTileY + 16; i++)
    {
        for (int j = startingTileX; j < startingTileX + 20; j++)
        {
            if (i >= 0 && i < 50 && j >= 0 && j < 50)
            {
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

void MapLoader::movePlayer(Vec2 pos)
{
    playerPos = pos;
}

void MapLoader::movePlayer(float x, float y)
{
    playerPos.x = x;
    playerPos.y = y;
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
    // glEnable(GL_TEXTURE_2D);
    // glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glPushMatrix();
    // glEnable(GL_ALPHA_TEST);
    // glAlphaFunc(GL_GREATER, 0.0f);
    // glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
    glColor4f(1.0f, 0.2f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
        // glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        // glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 250);
        // glTexCoord2f(1.0f, 0.0f); glVertex2i(250, 250);
        // glTexCoord2f(1.0f, 1.0f); glVertex2i(250, 0);
    glVertex2f(pos.x, pos.y);
    glVertex2f(pos.x, pos.y + 50);
    glVertex2f(pos.x + 50, pos.y + 50);
    glVertex2f(pos.x + 50, pos.y);
    glEnd();
    // glBindTexture(GL_TEXTURE_2D, 0);
    // glDisable(GL_ALPHA_TEST);
    // glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void EmptyTile::render(Vec2 &pos)
{
    // TODO: Add textures
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glPushMatrix();
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.grassTexture);
    glColor4f(1.0f, 0.8f, 1.0f, 0.8f);
    glBegin(GL_QUADS);
        // glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        // glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 250);
        // glTexCoord2f(1.0f, 0.0f); glVertex2i(250, 250);
        // glTexCoord2f(1.0f, 1.0f); glVertex2i(250, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(pos.x, pos.y);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(pos.x, pos.y + 50);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(pos.x + 50, pos.y + 50);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(pos.x + 50, pos.y);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}
