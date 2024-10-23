#include "MapScreen.h"
#include "../../../inputs/Inputs.h"
#include "../../SceneManager.h"

#include <X11/keysym.h>


MapScreen::MapScreen(int xres, int yres) : Scene(xres, yres)
{
    map = new MapLoader();
}

MapScreen::~MapScreen()
{
    release();
}

void MapScreen::init()
{
    map->setFileName("../../../test.map");
    map->LoadMapFile();
    map->loadTextures();
}

void MapScreen::update()
{
         if(get_key(XK_Left)) {
            map->setPlayerPos(Vec2(map->getPlayerPos().x - movement_speed,
                        map->getPlayerPos().y));
        }
        if(get_key(XK_Right)) {
            map->setPlayerPos(Vec2(map->getPlayerPos().x + movement_speed,
                        map->getPlayerPos().y));
        }
        if(get_key(XK_Up)) {
            map->setPlayerPos(Vec2(map->getPlayerPos().x,
                        map->getPlayerPos().y - movement_speed));
        }
        if(get_key(XK_Down)) {
            map->setPlayerPos(Vec2(map->getPlayerPos().x,
                        map->getPlayerPos().y + movement_speed));
        } 
}

void MapScreen::render()
{
    map->render();
}

void MapScreen::release()
{
    delete map;
    map = nullptr;
}