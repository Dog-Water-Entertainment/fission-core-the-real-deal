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

Image bossImages[5] = {
    "./assets/gameplay/enemy.png", // 9:21
    "./assets/gameplay/fight_radial.png", // 21:9
    "./assets/gameplay/fight_button.png", // 4:3
    "./assets/gameplay/run_button.png", // 4:3
    "./assets/gameplay/heal_button.png" // 4:3
};

class TextureContainer {
    public:
        GLuint enemy_texture;
        GLuint fight_radial_texture;
        GLuint fight_button_texture;
        GLuint run_button_texture;
        GLuint heal_button_texture;
        TextureContainer() {}
} static g;

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
    // Todo SETUP HEALTH STUFF


    glGenTextures(1, &g.enemy_texture);
    // For grass

    int w = bossImages[0].width;
	int h = bossImages[0].height;

    glBindTexture(GL_TEXTURE_2D, g.enemy_texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *enemyData = buildAlphaData(&bossImages[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, enemyData);
	free(enemyData);

    // Stuff right here to next comment
    w = bossImages[1].width;
	h = bossImages[1].height;

    glGenTextures(1, &g.fight_radial_texture);
    glBindTexture(GL_TEXTURE_2D, g.fight_radial_texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *fightData = buildAlphaData(&bossImages[1]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, fightData);
	free(fightData);

    
    // New Textures
    // for the tile map
    w = bossImages[2].width;
	h = bossImages[2].height;

    glGenTextures(1, &g.fight_button_texture);
    glBindTexture(GL_TEXTURE_2D, g.fight_button_texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *fightbData = buildAlphaData(&bossImages[2]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, fightbData);
	free(fightbData);

    
    w = bossImages[3].width;
	h = bossImages[3].height;

    glGenTextures(1, &g.run_button_texture);
    glBindTexture(GL_TEXTURE_2D, g.run_button_texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *runData = buildAlphaData(&bossImages[3]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, runData);
	free(runData);


    // Texture Here
    w = bossImages[4].width;
	h = bossImages[4].height;

    glGenTextures(1, &g.heal_button_texture);
    glBindTexture(GL_TEXTURE_2D, g.heal_button_texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *healData = buildAlphaData(&bossImages[4]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, healData);
	free(healData);
}

const static float speed = 2.0f;

void BossScene::Update()
{
    if(get_key(XK_y)) {
        m_pNextScene = new MapScreen(m_xres, m_yres);
    }

    // TODO Implement space bar for attacking
}

void BossScene::Render()
{
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g.fight_button_texture);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glTranslatef(m_xres - 200, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 250);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(250, 250);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(250, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void BossScene::Release()
{
}

// #include "BossScene.h"
