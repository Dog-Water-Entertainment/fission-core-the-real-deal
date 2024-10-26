#include "./BossScene.h"
#include "../MapScreen/MapScreen.h"
#include "../../../inputs/Inputs.h" 
#include <X11/keysym.h>

BossScene::BossScene(int xres, int yres)
{
    m_xres = xres;
    m_yres = yres;
    m_pNextScene = nullptr;
}

BossScene::~BossScene()
{
    Release();
}

void BossScene::Init()
{
}

void BossScene::Update()
{
    if(get_key(XK_y)) {
        m_pNextScene = new MapScreen(m_xres, m_yres);
    }
}

void BossScene::Render()
{
}

void BossScene::Release()
{
}

// #include "BossScene.h"
