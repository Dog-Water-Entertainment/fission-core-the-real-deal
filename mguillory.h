#ifndef _MGUILLORY_H_
#define _MGUILLORY_H_


#include <fstream>


// My Math Shit
class Vec2
{
public:
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);
    ~Vec2();

    Vec2 operator+(float rhs);
    Vec2 operator*(float rhs);
};

class Vec3
{
public:
    float x;
    float y;
    float z;

    Vec3();
    Vec3(float x, float y, float z);
    ~Vec3();

    Vec3 operator+(float rhs);
    Vec3 operator*(float rhs);

};

// Should be a value from -1 to 1
// 


// Tile-Map Stuff
class Tile 
{
public:
    char id;
    virtual void render(Vec2 &pos) = 0;
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
    void render(Vec2 &pos);
    ~DarkTile() {};
};

class FloorOneTile : public Tile {
    void render(Vec2 &pos);
    ~FloorOneTile() {};
};

class FloorTwoTile : public Tile {
    void render(Vec2 &pos);
    ~FloorTwoTile() {};
};

class FloorThreeTile : public Tile {
    void render(Vec2 &pos);
    ~FloorThreeTile() {};
};

class WallBottomLeft : public Tile {
    void render(Vec2 &pos);
    ~WallBottomLeft() {};
};

class WallBottomRight : public Tile {
    void render(Vec2 &pos);
    ~WallBottomRight() {};
};

class WallTopLeft : public Tile {
    void render(Vec2 &pos);
    ~WallTopLeft() {};
};

class WallTopRight : public Tile {
    void render(Vec2 &pos);
    ~WallTopRight() {};
};

class WallBottom : public Tile {
    void render(Vec2 &pos);
    ~WallBottom() {};
};

class WallTop : public Tile {
    void render(Vec2 &pos);
    ~WallTop() {};
};

class WallCornerLeft : public Tile {
    void render(Vec2 &pos);
    ~WallCornerLeft() {};
};

class WallCornerRight : public Tile {
    void render(Vec2 &pos);
    ~WallCornerRight() {};
};

class WallTLeft : public Tile {
    void render(Vec2 &pos);
    ~WallTLeft() {};
};

class WallTRight : public Tile {
    void render(Vec2 &pos);
    ~WallTRight() {};
};

class WallFourteen : public Tile {
    void render(Vec2 &pos);
    ~WallFourteen() {};
};

class WallEdgeLeft : public Tile {
    void render(Vec2 &pos);
    ~WallEdgeLeft() {};
};

class WallEdgeRight : public Tile {
    void render(Vec2 &pos);
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

#endif // _MGUILLORY_H_