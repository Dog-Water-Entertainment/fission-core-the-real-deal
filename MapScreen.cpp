#include "MapScreen.h"
#include "Inputs.h"
#include "SceneManager.h"
#include "BossScene.h"

#include <X11/keysym.h>

// Origin: Mykull Guillory

MapScreen::MapScreen(int xres, int yres)
{
    m_xres = xres;
    m_yres = yres;
    m_pNextScene = nullptr;
    map = new MapLoader();
}

MapScreen::~MapScreen()
{
    Release();
}

void MapScreen::Init()
{
    map->setFileName("test.map");
    map->LoadMapFile();
    map->loadTextures();
    movement_speed = 0.1;
}

void MapScreen::Update()
{
    Vec2 playerPos = map->getPlayerPos();

    if(get_key(XK_t)) {
        m_pNextScene = new BossScene(m_xres, m_yres);
    }

    if(isWalkable(map->getTileAt(playerPos.x, playerPos.y)) 
       && isWalkable(map->getTileAt(playerPos.x + 1, playerPos.y))
       && isWalkable(map->getTileAt(playerPos.x, playerPos.y + 1))
       && isWalkable(map->getTileAt(playerPos.x + 1, playerPos.y + 1))) { 

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
    } else {
        // currently nothing
    }

    if(get_key(XK_v)) {
        movement_speed = 0.8;
    }

}

void MapScreen::Render()
{
    map->render();
}

void MapScreen::Release()
{
    delete map;
    map = nullptr;
}
