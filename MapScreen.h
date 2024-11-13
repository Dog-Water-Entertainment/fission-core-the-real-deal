#ifndef _MAPSCREEN_H_
#define _MAPSCREEN_H_

#include "SceneManager.h"
#include "mguillory.h"

// Origin: Mykull Guillory

class MapScreen : public Scene
{
    MapLoader * map;
    float movement_speed;

    public:
        MapScreen(int xres, int yres);
        ~MapScreen() override;

        void Init() override;
        void Update() override;
        void Render() override;
        void Release() override;
};




#endif // _MAPSCREEN_H_
