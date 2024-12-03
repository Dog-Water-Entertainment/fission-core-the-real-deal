#include "MapScreen.h"
#include "Inputs.h"
#include "SceneManager.h"
#include "bmartinez.h"
#include "BossScene.h"
#include <stdlib.h>
#include <time.h>
#include <X11/keysym.h>

// Origin: Mykull Guillory

static Player playerData;

int getRandomNumberXX(int min, int max) {
	return rand() % (max - min + 1) + min;
}

MapScreen::MapScreen(int xres, int yres)
{
    m_xres = xres;
    m_yres = yres;
    m_pNextScene = nullptr;
    map = new MapLoader();
    playerData = Player(100, 10);
}

MapScreen::MapScreen(int xres, int yres, Vec2 playerPos)
{
    m_xres = xres;
    m_yres = yres;
    m_pNextScene = nullptr;
    map = new MapLoader();
    map->setPlayerPos(playerPos);
    playerData = Player(100, 10);
}

MapScreen::MapScreen(int xres, int yres, Vec2 playerPos, Player playerDataIn)
{
    srand(time(NULL));
    m_xres = xres;
    m_yres = yres;
    m_pNextScene = nullptr;
    map = new MapLoader();
    map->setPlayerPos(playerPos);

    playerData = playerDataIn;
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

static bool foundFirst = false;
static bool foundSecond = false;

void MapScreen::Update()
{
    Vec2 playerPos = map->getPlayerPos();

    if(playerPos.x >= 30 && playerPos.x <= 36 &&
       playerPos.y >= 11 && playerPos.y <= 16 && !foundFirst) {
        BossScene * next = new BossScene(m_xres, m_yres, playerData, Enemy(100, 8));
        next->setPlayerPos(playerPos.x, playerPos.y);
        m_pNextScene = next;
        foundFirst = true;
    }

    if(playerPos.x >= 15 && playerPos.x <= 18 &&
       playerPos.y >= 12 && playerPos.y <= 16 && !foundSecond) {
        BossScene * next = new BossScene(m_xres, m_yres, playerData, Enemy(200, 15));
        next->setPlayerPos(playerPos.x, playerPos.y);
        m_pNextScene = next;
        foundSecond = true;
    }

    if (get_key(XK_y)) {
        BossScene * next = new BossScene(m_xres, m_yres, playerData);
        next->setPlayerPos(playerPos.x, playerPos.y);
        m_pNextScene = next;
    }

    // 15, 16
    // 15, 12
    // 18, 12
    // 18, 16



    // TODO: Change this logic


    Vec2 newPos = playerPos;

    const int rarity = 700;

    if(get_key(XK_a)) {
        int randy = getRandomNumberXX(0, rarity);
        if (randy == 1) {
            BossScene * next = new BossScene(m_xres, m_yres, playerData, 
                Enemy(getRandomNumberXX(10, 20), getRandomNumberXX(0, 3)));
            next->setPlayerPos(playerPos.x, playerPos.y);
            m_pNextScene = next;
        }
        newPos.x -= movement_speed;
    }
    if(get_key(XK_d)) {        
        int randy = getRandomNumberXX(0, rarity);
        if (randy == 1) {
            BossScene * next = new BossScene(m_xres, m_yres, playerData, 
                Enemy(getRandomNumberXX(10, 20), getRandomNumberXX(0, 3)));
            next->setPlayerPos(playerPos.x, playerPos.y);
            m_pNextScene = next;
        }
        newPos.x += movement_speed;
    }
    if(get_key(XK_w)) {
        int randy = getRandomNumberXX(0, rarity);
        if (randy == 1) {
            BossScene * next = new BossScene(m_xres, m_yres, playerData,
                Enemy(getRandomNumberXX(10, 20), getRandomNumberXX(0, 3)));
            next->setPlayerPos(playerPos.x, playerPos.y);
            m_pNextScene = next;
        }
        newPos.y -= movement_speed;
    }
    if(get_key(XK_s)) {
        int randy = getRandomNumberXX(0, rarity);
        if (randy == 1) {
            BossScene * next = new BossScene(m_xres, m_yres, playerData, 
                Enemy(getRandomNumberXX(10, 20), getRandomNumberXX(0, 3)));
            next->setPlayerPos(playerPos.x, playerPos.y);
            m_pNextScene = next;
        }
        newPos.y += movement_speed;
    }

    if(isWalkable(map->getTileAt(newPos.y + 1, newPos.x))) {
        map->setPlayerPos(newPos);
    }

    // Bounding box to enter next scene
    // 36, 16
    // 30 16
    // 30, 11
    // 36, 11

    // cringe
    //if(get_key(XK_v)) {
    //    movement_speed = 0.8;
    //}
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
