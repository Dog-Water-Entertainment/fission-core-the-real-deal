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
};

#endif // _MGUILLORY_H_