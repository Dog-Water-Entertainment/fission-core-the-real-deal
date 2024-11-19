#ifndef _MGUILLORY_H_
#define _MGUILLORY_H_


#include <fstream>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <cmath>
#include <vector>
#include "Math.h"
#include "Shapes.h"



// Tile-Map Stuff
class Tile 
{
public:
    GLuint * texture;
    char id;
    void render(Vec2 &pos);
    char getTileType() { return id; }
    virtual ~Tile() {};
};

class BlockTile : public Tile 
{
public:
    void render(Vec2 &pos);
    ~BlockTile() {};
};

class EmptyTile : public Tile 
{
public:
    void render(Vec2 &pos);
    ~EmptyTile() {};
};

/*
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
*/

class DarkTile : public Tile {
    ~DarkTile() {};
};

class FloorOneTile : public Tile {
    ~FloorOneTile() {};
};

class FloorTwoTile : public Tile {
    ~FloorTwoTile() {};
};

class FloorThreeTile : public Tile {
    ~FloorThreeTile() {};
};

class WallBottomLeft : public Tile {
    ~WallBottomLeft() {};
};

class WallBottomRight : public Tile {
    ~WallBottomRight() {};
};

class WallTopLeft : public Tile {
    ~WallTopLeft() {};
};

class WallTopRight : public Tile {
    ~WallTopRight() {};
};

class WallBottom : public Tile {
    ~WallBottom() {};
};

class WallTop : public Tile {
    ~WallTop() {};
};

class WallCornerLeft : public Tile {
    ~WallCornerLeft() {};
};

class WallCornerRight : public Tile {
    ~WallCornerRight() {};
};

class WallTLeft : public Tile {
    ~WallTLeft() {};
};

class WallTRight : public Tile {
    ~WallTRight() {};
};

class WallFourteen : public Tile {
    ~WallFourteen() {};
};

class WallEdgeLeft : public Tile {
    ~WallEdgeLeft() {};
};

class WallEdgeRight : public Tile {
    ~WallEdgeRight() {};
};


class MapLoader
{
private:
    std::string mapFileName;
    Tile * map[50][50];
    Vec2 center;
    Vec2 playerPos;
public:
    MapLoader();
    MapLoader(const char* filename);
    ~MapLoader();

    void loadTextures();

    void setFileName(const char* filename);

    void LoadMapFile();
    void render();

    void setPlayerPos(Vec2 pos);
    void setPlayerPos(float x, float y);

    Vec2 getPlayerPos();

    void movePlayer(Vec2 pos);
    void movePlayer(float x, float y);

    Tile * getTileAt(float x, float y);
};

bool isWalkable(Tile * tile);

namespace Physics2d {
    std::vector<Rectangle> raycast(std::vector<Rectangle> rects, Vec2 origin, float angle, float distance);
}

#endif // _MGUILLORY_H_