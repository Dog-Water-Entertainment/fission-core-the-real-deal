#ifndef _MAPSCREEN_H_
#define _MAPSCREEN_H_

#include "../../SceneManager.h"
#include "../../../mguillory.h"


class MapScreen : public Scene
{
    MapLoader * map;
    const float movement_speed = 0.1f;

    public:
        MapScreen(int xres, int yres);
        ~MapScreen();

        void init();
        void update();
        void render();
        void release();
};




#endif // _MAPSCREEN_H_