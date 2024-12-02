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

MapScreen::MapScreen(int xres, int yres, Vec2 playerPos)
{
    m_xres = xres;
    m_yres = yres;
    m_pNextScene = nullptr;
    map = new MapLoader();
    map->setPlayerPos(playerPos);
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


    // TODO: Change this logic
    if(get_key(XK_t)) {
        BossScene * next = new BossScene(m_xres, m_yres);
        next->setPlayerPos(playerPos.x, playerPos.y);
        m_pNextScene = next;
    }

    Vec2 newPos = playerPos;

    if(get_key(XK_a)) {
        newPos.x -= movement_speed;
    }
    if(get_key(XK_d)) {
        newPos.x += movement_speed;
    }
    if(get_key(XK_w)) {
        newPos.y -= movement_speed;
    }
    if(get_key(XK_s)) {
        newPos.y += movement_speed;
    }

    if(isWalkable(map->getTileAt(newPos.y + 1, newPos.x + 1))) {
        map->setPlayerPos(newPos);
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
