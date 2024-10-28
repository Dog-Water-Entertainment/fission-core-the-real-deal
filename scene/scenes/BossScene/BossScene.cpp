#include "./BossScene.h"
#include "../MapScreen/MapScreen.h"
#include "../../../inputs/Inputs.h"
#include "../../../math/Shapes.h"
#include "../../../math/Math.h"
#include <X11/keysym.h>
#include <stdio.h>

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
    Vec2 mousePos = get_mouse_pos();
    printf("Mouse Pos: %f, %f\n", mousePos.x, mousePos.y);

    if (get_mouse_inpulse()) {
        printf("Mouse Pressed\n");
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
