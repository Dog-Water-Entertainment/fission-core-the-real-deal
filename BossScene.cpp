#include "BossScene.h"
#include "MapScreen.h"
#include "Inputs.h"
#include "Shapes.h"
#include "Math.h"
#include "Image.h"
#include "fonts.h"
#include "bmartinez.h"
#include "efarmer.h"
#include <cmath>
#include <X11/keysym.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Origin: Mykull Guillory

static int backwards = 0;
static int input_inpulse = 0;
static int first = 1;
static int difficulty = 20;
static int playerTurn = 1;
static int move = 0;
static int counter = 0;
static int firstPass = 1;

Image bossImages[5] = {
    "./assets/gameplay/enemy.png", // 9:21
    "./assets/gameplay/fight_radial.png", // 21:9
    "./assets/gameplay/fight_button.png", // 4:3
    "./assets/gameplay/run_button.png", // 4:3
    "./assets/gameplay/heal_button.png" // 4:3
};

enum Buttons {
	FIGHT,
	RUN,
	HEAL
};

int getRandomNumber(int min, int max) {
	return rand() % (max - min + 1) + min;
}

class TextureContainer {
    public:
        GLuint enemy_texture;
        GLuint fight_radial_texture;
        GLuint fight_button_texture;
        GLuint run_button_texture;
        GLuint heal_button_texture;
        TextureContainer() {}
} static g;

static int fighting_offset = 0;


class MouseContainer {
	public:
		Vec2 position;
		bool leftButton;
		bool rightButton;
		MouseContainer() {
			position = Vec2(0, 0);
			leftButton = false;
			rightButton = false;
		}
};

static MouseContainer currMouse;

BossScene::BossScene(int xres, int yres)
{
    m_xres = xres;
    m_yres = yres;
	m_pNextScene = nullptr;
	fighting = false;
	player = Player(100, 10);
	enemy = Enemy(200, 15);
}

BossScene::BossScene(int xres, int yres, Player playerData)
{
	m_xres = xres;
	m_yres = yres;
	m_pNextScene = nullptr;
	player = playerData;
	fighting = false;
}

BossScene::BossScene(int xres, int yres, Player playerData, Enemy enemyData)
{
	m_xres = xres;
	m_yres = yres;
	m_pNextScene = nullptr;
	player = playerData;
	fighting = false;
	enemy = enemyData;
}

BossScene::~BossScene()
{
    Release();
}

int getButtonHover(int xres, int yres) {
	// Check Fight
	if (currMouse.position.x >= (xres / 2) - 100 && 
		currMouse.position.x <= (xres / 2) + 100 &&
		currMouse.position.y >= yres-598 && 
		currMouse.position.y <= 152.0f) {
		return FIGHT;
	}
	// Check Run
	if (currMouse.position.x >= xres - 250 && 
		currMouse.position.x <= xres - 50 &&
		currMouse.position.y >= 2.0f && 
		currMouse.position.y <= 152.0f) {
		return RUN;
	}

	// Check Heal
	if (currMouse.position.x >= 50 && currMouse.position.x <= 250 &&
		currMouse.position.y >= 2.0f && currMouse.position.y <= 152.0f) {
		return HEAL;
	}

	// Return -1 if no button is hovered
	return -1;
}

void BossScene::Init()
{
	srand(time(NULL));

	backwards = 0;
	input_inpulse = 0;
	first = 1;
	playerTurn = 1;
	move = 0;
	counter = 0;
	firstPass = 1;

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

	DialogManager::promptDialog(
		"Enemy", 
		{
			"An enemy has appeared!"
		}, 
		m_xres / 2 - 300, 
		50, 
		0x00ffffff
	);
}

const static float speed = 2.0f;

bool BossScene::isFighting() 
{
	return fighting;
}

std::string stringify(int num, Enemy e) {
	return num == 1 ? "Enemy attacked the player for " + std::to_string(e.dmgDeal) + " damage!" : "Enemy missed!";
}


void BossScene::Update()
{
	if (get_key(XK_space) && first) {
		input_inpulse = 1;
		first = false;
	} else {
		input_inpulse = 0;
	}
	if (!get_key(XK_space)) {
		first = true;
	}

	if(input_inpulse && DialogManager::isDialogActive()) {
		DialogManager::tryAdvanceDialog(false);
	}

	if(DialogManager::isDialogActive()) {
		return;
	}

	if (enemy.HP <= 0) {
		DialogManager::promptDialog(
			"Enemy", 
			{
				"Enemy has been defeated!"
			}, 
			m_xres / 2 - 300, 
			50, 
			0x00ffffff
		);
		exitScene();
		return;
	}


	if (move > 2) {
		playerTurn = !playerTurn;
		move = 0;
	}

	if (playerTurn) {
		int multiplier = 1;
		currMouse.position = get_mouse_pos();
		int mouseClick = get_mouse_inpulse();
		if(get_key(XK_y)) {
			m_pNextScene = new MapScreen(m_xres, m_yres);
		}
		if(isFighting()) {
			if (fighting_offset < 420 && backwards == 0) {
				fighting_offset += difficulty;
			} else if (fighting_offset >= 420) {
				backwards = 1;
				fighting_offset -= difficulty;
			} else if (fighting_offset > 0 && backwards == 1) {
				fighting_offset -= difficulty;
			} else if (fighting_offset <= 0) {
				backwards = 0;
				fighting_offset += difficulty;
			}

			if (input_inpulse) {
				if (fighting_offset >= 197 && fighting_offset <= 219) {
					multiplier = 4;
				} else if (fighting_offset <= 105 || fighting_offset >= 315) {
					multiplier = 0;
				}
				
				DialogManager::promptDialog(
					"Player " + std::to_string(counter), 
					{
						"Player attacked the enemy for " + std::to_string(player.dmgDeal * multiplier) + " damage!"
					}, 
					m_xres / 2 - 300, 
					50, 
					0x00ffffff
				);
				counter++;
				player.Attack(enemy, player.dmgDeal * multiplier);
				move++;
				fighting = false;
			}
		}


		if(mouseClick && !fighting) {
			int button = getButtonHover(m_xres, m_yres);
			if(button == FIGHT) {
				// Fight
				fighting = true;
			} else if(button == RUN) {
				//Run (if lucky)
                int randNum = (rand() % 10) + 1;
                    if (randNum == 1) {
                        exitScene();
                        DialogManager::promptDialog("Terrance",
                                { "Successfully ran!", "Quite lucky!"},
                                m_xres / 2 - 300, 100, 0x00ffffff);
                    } else {
                        DialogManager::promptDialog("Terrance",
                                {"You ran and slipped, not great."},
                                m_xres / 2 - 300, 100, 0x00ffffff);
                        move++;
                    }
			} else if(button == HEAL) {
				// Heal
				player.HP += 10;
				move++;
			}
		}
	} else {
		// Enemy Turn

		// Please fix logic
		int array[3];
		for (int i = 0; i < 3; i++) {
			if(getRandomNumber(0, 4) != 1) {
				enemy.Attack(player, enemy.dmgDeal, *dead);
				array[i] = 1;
			} else {
				array[i] = 0;
			}
			move++;
		}

		DialogManager::promptDialog(
			"Enemy", 
			{
				stringify(array[0], enemy),
				stringify(array[1], enemy),
				stringify(array[2], enemy)
			}, 
			m_xres / 2 - 300, 
			50, 
			0x00ffffff
		);
	}
}

void BossScene::Render()
{
	if (DialogManager::isDialogActive() && firstPass) {
		return;
	} else {
		firstPass = 0;
	}


	// Fight Button
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g.fight_button_texture);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glTranslatef((m_xres / 2) - 100, 2.0f, 0.0f);
	if(getButtonHover(m_xres, m_yres) == FIGHT) {
		glScalef(1.1f, 1.1f, 1.1f);
		glTranslatef(-10, -7.5, 0);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 150);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(200, 150);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(200, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// Heal Button
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g.heal_button_texture);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glTranslatef(50, 2.0f, 0.0f);
	if(getButtonHover(m_xres, m_yres) == HEAL) {
		glScalef(1.1f, 1.1f, 1.1f);
		glTranslatef(-10, -7.5, 0);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 150);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(200, 150);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(200, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// Run Button
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g.run_button_texture);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glTranslatef(m_xres - 250, 2.0f, 0.0f);
	if(getButtonHover(m_xres, m_yres) == RUN) {
		glScalef(1.1f, 1.1f, 1.1f);
		glTranslatef(-10, -7.5, 0);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 150);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(200, 150);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(200, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// Center Radial
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g.fight_radial_texture);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glTranslatef(m_xres / 2 - 210, 170.0f, 0.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 100);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(420, 100);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(420, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	// Enemy texture
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g.enemy_texture);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glTranslatef(m_xres / 2 - 75, 200.0f, 0.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 400);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(150, 400);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(150, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);

	if(isFighting()) {
		// Draw the little radial thing
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glColor4f(0.8f, 1.0f, 0.0f, 0.8f);
		glTranslatef(m_xres / 2 - 210 + fighting_offset, 170.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(0, 100);
			glVertex2i(5, 100);
			glVertex2i(5, 0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	}

	// Draw Health Text
	Rect r;
	r.bot = 190;
	r.left = m_xres / 2 + 230;
	r.center = 0;
	ggprint16(&r, 16, 0x00ffffff, "Health: %d/%d", player.HP, player.MaxHP);

	r.bot = m_yres - 50;
	r.left = 10;
	r.center = 0;
	ggprint16(&r, 16, 0x00ffffff, "Enemy Health: %d/%d", enemy.HP, enemy.MaxHP);
}

void BossScene::Release()
{
}

void BossScene::exitScene()
{
	m_pNextScene = new MapScreen(m_xres, m_yres, oldPlayerPos, player);
}

void BossScene::setPlayerPos(float x, float y)
{
	oldPlayerPos = Vec2(x, y);
}

void BossScene::setDead(bool * dead)
{
	this->dead = dead;
}

// #include "BossScene.h"
