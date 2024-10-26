#include "./BossScene.h"
#include "../MapScreen/MapScreen.h"
#include "../../../inputs/Inputs.h"
#include "../../../math/Shapes.h"
#include "../../../math/Math.h"
#include <X11/keysym.h>

BossScene::BossScene(int xres, int yres)
{
    m_xres = xres;
    m_yres = yres;
    m_rectangle = Rectangle(Vec2(m_xres/2, m_yres/2), 100, 100);
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
    m_rectangle.rotateAroundLocal(0.01f);
}

void BossScene::Render()
{
    m_rectangle.draw();
}

void BossScene::Release()
{
}

// #include "BossScene.h"
