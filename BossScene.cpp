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
    m_rectangle = Rectangle(Vec2(m_xres/2, m_yres/2), 50, 50);
    m_pNextScene = nullptr;
    texture = 0;
}

BossScene::~BossScene()
{
    Release();
}

void BossScene::Init()
{
    
    Image playerImg = Image("./assets/playerTopDown.png");

    glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &playerTexture);

	int w = playerImg.width;
	int h = playerImg.height;
	//
	glBindTexture(GL_TEXTURE_2D, playerTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, playerImg.data);

		glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *silhouetteData = buildAlphaData(&playerImg);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
								GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
    
    
/*
    Image img = Image("./assets/darktile.png");

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
*/

    m_rectangle.setTexture(&silhouetteTexture);
    glDisable(GL_TEXTURE_2D);
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
    degubLine.draw();
}

void BossScene::Release()
{
}

// #include "BossScene.h"
