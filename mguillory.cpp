// Michael Guillory
// 10/03/2024
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
#include <exception>
#include <vector>
#include <typeinfo>
#include "Math.h"
#include "Image.h"
#include "Shapes.h"

static Image img[19] = {
	"./images/map_textures/grass.jpg",
    "./images/map_textures/stone.png",
    "./assets/darktile.png",
    "./assets/floor1.png",
    "./assets/floor2.png",
    "./assets/floor3.png",
    "./assets/wall_bottom_left.png",
    "./assets/wall_bottom_right.png",
    "./assets/wall_top_left.png",
    "./assets/wall_top_right.png",
    "./assets/wall_bottom.png",
    "./assets/wall_top.png",
    "./assets/wall_corner_left.png",
    "./assets/wall_corner_right.png",
    "./assets/wall_t_left.png",
    "./assets/wall_t_right.png",
    "./assets/wall14.png",
    "./assets/wall_edge_left.png",
    "./assets/wall_edge_right.png"
};

class TileMapError : public std::exception
{
    std::string message;
public:
    TileMapError(std::string msg) 
    {
        message = msg;
    }
    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

class global 
{
public:
    GLuint grassTexture;
    GLuint stoneTexture;

    GLuint darkTile;
    GLuint floor1;
    GLuint floor2;
    GLuint floor3;
    GLuint wall_bottom_left;
    GLuint wall_bottom_right;
    GLuint wall_top_left;
    GLuint wall_top_right;
    GLuint wall_bottom;
    GLuint wall_top;
    GLuint wall_corner_left;
    GLuint wall_corner_right;
    GLuint wall_t_left;
    GLuint wall_t_right;
    GLuint wall14;
    GLuint wall_edge_left;
    GLuint wall_edge_right;


    global() {
        //nothing
    }
} static g;

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
    center = Vec2(0, 0);
    playerPos = Vec2(25, 25);
}

MapLoader::MapLoader(const char *filename)
{
    mapFileName = filename;
    center = Vec2(0, 0);
    playerPos = Vec2(25, 25);
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
    // This is going to be long jesus christ
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

    // Stuff right here to next comment
    w = img[1].width;
	h = img[1].height;

    glGenTextures(1, &g.stoneTexture);
    glBindTexture(GL_TEXTURE_2D, g.stoneTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *stoneData = buildAlphaData(&img[1]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, stoneData);
	free(stoneData);

    
    // New Textures
    // for the tile map
    w = img[2].width;
	h = img[2].height;

    glGenTextures(1, &g.darkTile);
    glBindTexture(GL_TEXTURE_2D, g.darkTile);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *darkData = buildAlphaData(&img[2]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, darkData);
	free(darkData);

    
    w = img[3].width;
	h = img[3].height;

    glGenTextures(1, &g.floor1);
    glBindTexture(GL_TEXTURE_2D, g.floor1);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *floor1Data = buildAlphaData(&img[3]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, floor1Data);
	free(floor1Data);


    // Texture Here
    w = img[4].width;
	h = img[4].height;

    glGenTextures(1, &g.floor2);
    glBindTexture(GL_TEXTURE_2D, g.floor2);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *floor2Data = buildAlphaData(&img[4]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, floor2Data);
	free(floor2Data);


    // Texture Here
    w = img[5].width;
	h = img[5].height;

    glGenTextures(1, &g.floor3);
    glBindTexture(GL_TEXTURE_2D, g.floor3);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *floor3Data = buildAlphaData(&img[5]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, floor3Data);
	free(floor3Data);


    // Texture Here
    w = img[6].width;
    h = img[6].height;

    glGenTextures(1, &g.wall_bottom_left);
    glBindTexture(GL_TEXTURE_2D, g.wall_bottom_left);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *wall_bottom_leftData = buildAlphaData(&img[6]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_bottom_leftData);
    free(wall_bottom_leftData);


    // Texture Here
    w = img[7].width;
    h = img[7].height;

    glGenTextures(1, &g.wall_bottom_right);
    glBindTexture(GL_TEXTURE_2D, g.wall_bottom_right);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *wall_bottom_rightData = buildAlphaData(&img[7]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_bottom_rightData);
    free(wall_bottom_rightData);


    // Texture Here
    w = img[8].width;
    h = img[8].height;

    glGenTextures(1, &g.wall_top_left);
    glBindTexture(GL_TEXTURE_2D, g.wall_top_left);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_top_leftData = buildAlphaData(&img[8]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_top_leftData);
    free(wall_top_leftData);


    // Texture Here
    w = img[9].width;
    h = img[9].height;

    glGenTextures(1, &g.wall_top_right);
    glBindTexture(GL_TEXTURE_2D, g.wall_top_right);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_top_rightData = buildAlphaData(&img[9]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_top_rightData);
    free(wall_top_rightData);


    // Texture Here
    w = img[10].width;
    h = img[10].height;

    glGenTextures(1, &g.wall_bottom);
    glBindTexture(GL_TEXTURE_2D, g.wall_bottom);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_bottomData = buildAlphaData(&img[10]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_bottomData);
    free(wall_bottomData);


    // Texture Here
    w = img[11].width;
    h = img[11].height;

    glGenTextures(1, &g.wall_top);
    glBindTexture(GL_TEXTURE_2D, g.wall_top);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_topData = buildAlphaData(&img[11]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_topData);
    free(wall_topData);


    // Texture Here
    w = img[12].width;
    h = img[12].height;

    glGenTextures(1, &g.wall_corner_left);
    glBindTexture(GL_TEXTURE_2D, g.wall_corner_left);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_corner_leftData = buildAlphaData(&img[12]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_corner_leftData);
    free(wall_corner_leftData);


    // Texture Here
    w = img[13].width;
    h = img[13].height;

    glGenTextures(1, &g.wall_corner_right);
    glBindTexture(GL_TEXTURE_2D, g.wall_corner_right);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_corner_rightData = buildAlphaData(&img[13]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_corner_rightData);
    free(wall_corner_rightData);


    // Texture Here
    w = img[14].width;
    h = img[14].height;

    glGenTextures(1, &g.wall_t_left);
    glBindTexture(GL_TEXTURE_2D, g.wall_t_left);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_t_leftData = buildAlphaData(&img[14]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_t_leftData);
    free(wall_t_leftData);


    // Texture Here
    w = img[15].width;
    h = img[15].height;

    glGenTextures(1, &g.wall_t_right);
    glBindTexture(GL_TEXTURE_2D, g.wall_t_right);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_t_rightData = buildAlphaData(&img[15]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_t_rightData);
    free(wall_t_rightData);


    // Texture Here
    w = img[16].width;
    h = img[16].height;

    glGenTextures(1, &g.wall14);
    glBindTexture(GL_TEXTURE_2D, g.wall14);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall14Data = buildAlphaData(&img[16]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall14Data);

    free(wall14Data);


    // Texture Here
    w = img[17].width;
    h = img[17].height;

    glGenTextures(1, &g.wall_edge_left);
    glBindTexture(GL_TEXTURE_2D, g.wall_edge_left);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_edge_leftData = buildAlphaData(&img[17]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_edge_leftData);
    free(wall_edge_leftData);


    // Texture Here
    w = img[18].width;
    h = img[18].height;

    glGenTextures(1, &g.wall_edge_right);
    glBindTexture(GL_TEXTURE_2D, g.wall_edge_right);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *wall_edge_rightData = buildAlphaData(&img[18]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, wall_edge_rightData);
    free(wall_edge_rightData);

    glBindTexture(GL_TEXTURE_2D, 0);
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

    if (!mapFile.is_open()) {
        throw TileMapError("Could not open map file");
        return;
    }

    std::string line;
    int i = 0;
    while (std::getline(mapFile, line) && i < tilex) {
        int j = 0;
        for (char &ch : line) {
            if (j < tiley) {
                if (ch == 'a') {
                    map[i][j] = new DarkTile();
                    map[i][j]->id = 'a';
                    map[i][j]->texture = &g.darkTile;
                } else if (ch == 'b') {
                    map[i][j] = new FloorOneTile();
                    map[i][j]->id = 'b';
                    map[i][j]->texture = &g.floor1;
                } else if (ch == 'c') {
                    map[i][j] = new FloorTwoTile();
                    map[i][j]->id = 'c';
                    map[i][j]->texture = &g.floor2;
                } else if (ch == 'd') {
                    map[i][j] = new FloorThreeTile();
                    map[i][j]->id = 'd';    
                    map[i][j]->texture = &g.floor3;
                } else if (ch == 'e') {
                    map[i][j] = new WallFourteen();
                    map[i][j]->id = 'e';
                    map[i][j]->texture = &g.wall14;
                } else if (ch == 'f') {
                    map[i][j] = new WallBottom();
                    map[i][j]->id = 'f';
                    map[i][j]->texture = &g.wall_bottom;
                } else if (ch == 'g') {
                    map[i][j] = new WallBottomLeft();
                    map[i][j]->id = 'g';
                    map[i][j]->texture = &g.wall_bottom_left;
                } else if (ch == 'h') {
                    map[i][j] = new WallBottomRight();
                    map[i][j]->id = 'h';
                    map[i][j]->texture = &g.wall_bottom_right;
                } else if (ch == 'i') {
                    map[i][j] = new WallCornerLeft();
                    map[i][j]->id = 'i';
                    map[i][j]->texture = &g.wall_corner_left;
                } else if (ch == 'j') {
                    map[i][j] = new WallCornerRight();
                    map[i][j]->id = 'j';
                    map[i][j]->texture = &g.wall_corner_right;
                } else if (ch == 'k') {
                    map[i][j] = new WallEdgeLeft();
                    map[i][j]->id = 'k';
                    map[i][j]->texture = &g.wall_edge_left;
                } else if (ch == 'l') {
                    map[i][j] = new WallEdgeRight();
                    map[i][j]->id = 'l';
                    map[i][j]->texture = &g.wall_edge_right;
                } else if (ch == 'm') {
                    map[i][j] = new WallTLeft();
                    map[i][j]->id = 'm';
                    map[i][j]->texture = &g.wall_t_left;
                } else if (ch == 'n') {
                    map[i][j] = new WallTRight();
                    map[i][j]->id = 'n';
                    map[i][j]->texture = &g.wall_t_right;
                } else if (ch == 'o') {
                    map[i][j] = new WallTop();
                    map[i][j]->id = 'o';
                    map[i][j]->texture = &g.wall_top;
                } else if (ch == 'p') {
                    map[i][j] = new WallTopLeft();
                    map[i][j]->id = 'p';
                    map[i][j]->texture = &g.wall_top_left;
                } else if (ch == 'q') {
                    map[i][j] = new WallCornerLeft();
                    map[i][j]->id = 'q';
                    map[i][j]->texture = &g.wall_corner_left;
                } else if (ch == ' ') {
                    continue;
                } else {
                    throw TileMapError("Invalid character in map file");
                    return;
                }
                j++;
            } else {
                throw TileMapError("Map file is too large");
                break;
            }
        }
        i++;
    }

    mapFile.close();
}

void MapLoader::render()
{
    // Calculate the center tile based on the player's position

    // Do checks to make sure the player is within bounds
    if (playerPos.x < 0 || playerPos.x >= 50 || playerPos.y < 0 || playerPos.y >= 50) {
        return;
    }

    Vec2 tileCenter = Vec2(floor(playerPos.x), 
                           floor(playerPos.y));
    int startingTileY = tileCenter.y - 8; // Adjusted for correct axis
    int startingTileX = tileCenter.x - 10; // Adjusted for correct axis

    // Calculate the offset for smooth movement
    Vec2 offset = Vec2(playerPos.x - floor(playerPos.x), 
                       playerPos.y - floor(playerPos.y));

    // Calculate the initial render position
    Vec2 currentRenderPos = Vec2(-100 - offset.x * 50, 700 + offset.y * 50);

    // Render only the visible tiles plus a buffer
    for (int i = startingTileY; i < startingTileY + 16; i++) {
        if (i < 0 || i >= 50) {
            currentRenderPos.y -= 50;
            continue;
        } // Skip out-of-bounds rows
        for (int j = startingTileX; j < startingTileX + 20; j++) {
            if (j >= 0 && j < 50) {
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
    playerPos.x += pos.x;
    playerPos.y += pos.y;
}

void MapLoader::movePlayer(float x, float y)
{
    playerPos.x += x;
    playerPos.y += y;
}

Tile * MapLoader::getTileAt(float x, float y)
{

    int tilex = floor(x);
    int tiley = floor(y);
    if (x >= 0 && x < 50 && y >= 0 && y < 50) {
        return map[tilex][tiley];
    }
    return nullptr;
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

void Tile::render(Vec2 &pos) {
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, *texture);
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(pos.x, pos.y);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(pos.x, pos.y + 50);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(pos.x + 50, pos.y + 50);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(pos.x + 50, pos.y);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

bool isWalkable(Tile * tile) {
    char id = tile->getTileType();
    return id == 'b' || id == 'c' || id == 'd';
}


/*
 * ========================================
 *
 *
 *
 *            2D    RayCast
 *
 *
 *
 *
 * ========================================
 */

// Should return a vector of shapes that were hit by the ray
std::vector<Shape> Physics2d::raycast(std::vector<Shape> shapes, Vec2 origin, float angle, float distance)
{
    std::vector<Shape> hits;
    Vec2 end = Vec2(origin.x + distance * cos(angle), origin.y + distance * sin(angle));
    for (Shape &shape : shapes) {
        if (typeid(shape) == typeid(Rectangle)) {
            // Nothing for right now
        }
    }
    return hits;
}
