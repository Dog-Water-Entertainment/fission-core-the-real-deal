#include "BossScene.h"
#include "MapScreen.h"
#include "Inputs.h"
#include "Shapes.h"
#include "Math.h"
#include "Image.h"
#include <cmath>
#include <X11/keysym.h>
#include <stdio.h>

// Origin: Mykull Guillory

BossScene::BossScene(int xres, int yres)
{
    degubLine = Line(Vec2(100, 200), Vec2(400, 500), 5);
    m_xres = xres;
    m_yres = yres;
    sceneObjects.push_back(Rectangle(Vec2(m_xres/2, m_yres/2), 50, 50));
    m_pNextScene = nullptr;
    texture = 0;
}

BossScene::~BossScene()
{
    Release();
}

void BossScene::Init()
{

    Image img = Image("./images/walk.gif");

    int w = img.width;
	int h = img.height;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *imgData = buildAlphaData(&img);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	free(imgData);

    sceneObjects[0].setTexture(&texture);
}

const static float speed = 2.0f;

void BossScene::Update()
{
    if(get_key(XK_y)) {
        m_pNextScene = new MapScreen(m_xres, m_yres);
    }

    // MOVEMENT 
    if(get_key(XK_w)) {
        sceneObjects[0].move(Vec2(0, speed));
    }
    if(get_key(XK_s)) {
        sceneObjects[0].move(Vec2(0, -speed));
    }
    if(get_key(XK_a)) {
        sceneObjects[0].move(Vec2(-speed, 0));
    }
    if(get_key(XK_d)) {
        sceneObjects[0].move(Vec2(speed, 0));
    }

    // get the angle from the rectangle to the mouse

    // ROTATION
    Vec2 mousePos = get_mouse_pos();
    sceneObjects[0].rotation = atan2(mousePos.y - m_rectangle.getPosition().y, mousePos.x - m_rectangle.getPosition().x);
}

void BossScene::Render()
{
    sceneObjects[0].draw();
    degubLine.draw();
}

void BossScene::Release()
{
}

// #include "BossScene.h"
