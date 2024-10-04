#include <fstream>

#ifndef MGUILLORY_H
#define MGUILLORY_H

class MapLoader
{
private:
    std::string mapFileName;
public:
    MapLoader();
    MapLoader(const char* filename);
    ~MapLoader();

    void LoadMapFile(const char* filename);
};

#endif