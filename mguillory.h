#ifndef _MGUILLORY_H_
#define _MGUILLORY_H_


#include <fstream>

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
public:
    MapLoader();
    MapLoader(const char* filename);
    ~MapLoader();

    void LoadMapFile(const char* filename);
    void render();
};

#endif