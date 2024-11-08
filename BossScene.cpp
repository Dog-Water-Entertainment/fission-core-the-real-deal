#include "BossScene.h"
#include "MapScreen.h"
#include "Inputs.h"
#include "Shapes.h"
#include "Math.h"
#include "Image.h"
#include <cmath>
#include <X11/keysym.h>
#include <stdio.h>

BossScene::BossScene(int xres, int yres)
{
    m_xres = xres;
    m_yres = yres;
    m_rectangle = Rectangle(Vec2(m_xres/2, m_yres/2), 50, 50);
    m_pNextScene = nullptr;
}

BossScene::~BossScene()
{
    Release();
}

void BossScene::Init()
{
    GLuint texture;
    loadTexture("assets/wall14.png", texture);

    m_rectangle.setTexture(&texture);
}

const static float speed = 2.0f;

void BossScene::Update()
{
    if(get_key(XK_y)) {
        m_pNextScene = new MapScreen(m_xres, m_yres);
    }

    // MOVEMENT 
    if(get_key(XK_w)) {
        m_rectangle.move(Vec2(0, speed));
    }
    if(get_key(XK_s)) {
        m_rectangle.move(Vec2(0, -speed));
    }
    if(get_key(XK_a)) {
        m_rectangle.move(Vec2(-speed, 0));
    }
    if(get_key(XK_d)) {
        m_rectangle.move(Vec2(speed, 0));
    }

    // get the angle from the rectangle to the mouse

    // ROTATION
    Vec2 mousePos = get_mouse_pos();
    m_rectangle.rotation = atan2(mousePos.y - m_rectangle.getPosition().y, mousePos.x - m_rectangle.getPosition().x);
}

void BossScene::Render()
{
    m_rectangle.draw();
}

void BossScene::Release()
{
}

// #include "BossScene.h"
