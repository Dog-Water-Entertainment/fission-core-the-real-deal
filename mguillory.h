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
};

// Should be a value from -1 to 1
// 
class NormalizedCoord 
{
public:
    Vec2 coord;
    Vec2 worldSpace;

    NormalizedCoord();
    NormalizedCoord(Vec2 coord);
    NormalizedCoord(float x, float y);

    Vec2 getWorldCoord();
    Vec2 getCoord();
    void setCoord(Vec2 coord);
    void setCoord(float x, float y);
    void setWorldSpace(float x, float y);
    void setWorldSpace(Vec2 space);



    ~NormalizedCoord();
};

class ScreenCoord 
{
    Vec2 coord;
    Vec2 worldSpace;

    ScreenCoord(float x, float y);
    ScreenCoord(Vec2 coord);
    ~ScreenCoord();

    void setWorldSpace(float x, float y);
    void setWorldSpace(Vec2 space);
};


// Tile-Map Stuff

class Tile 
{
public:
    virtual void render() = 0;
};

class BlockTile : public Tile 
{
public:
    void render();
};

class EmptyTile : public Tile 
{
public:
    void render();
};

class MapLoader
{
private:
    std::string mapFileName;
    Tile * map[50][50];
    Vec2 center;
public:
    MapLoader();
    MapLoader(const char* filename);
    ~MapLoader();

    void LoadMapFile(const char* filename);
    void render();
};

#endif // _MGUILLORY_H_