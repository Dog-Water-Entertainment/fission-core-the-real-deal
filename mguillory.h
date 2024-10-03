#ifdef _MGUILLORY_H_
#define _MGUILLORY_H_


#include <fstream>

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

#endif // _MGUILLORY_H_