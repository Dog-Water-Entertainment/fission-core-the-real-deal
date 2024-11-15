//author: brian martinez
//original start date: ?
//current date: 10/22/24
#include <GL/glx.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <X11/keysym.h>
#include "bmartinez.h"
#include "Inputs.h"
#include "fonts.h"
#include "efarmer.h"
#include "MapScreen.h"
// Forward Declarations////////////////////////////////////////////////////////
class Item;
class Player;
class Enemy;
// ITEM STUFF//////////////////////////////////////////////////////////////////
Item::Item(int ID) : itemID(ID) {} 
void Item::Use(int ID, Player& a)
{


}

void Item::Use(int ID, Enemy& a)
{


}
///////////////////////////////////////////////////////////////////////////////
///
//PLAYER STUFF/////////////////////////////////////////////////////////////////
Player::Player(Stat hp, Stat dmg) : MaxHP(hp), HP(hp), dmgDeal(dmg) {}
Player::Player() : MaxHP(50), HP(50), dmgDeal(2) {}

void Player::Attack(Enemy &a, Stat dmg)
{
    a.HP -= dmg;
    if (a.HP < 0)
        a.HP = 0;
}
void Player::Heal(Stat heals) 
{
    HP += heals;
    if (HP > MaxHP)
        HP = MaxHP;
}
///////////////////////////////////////////////////////////////////////////////
///
//ENEMY STUFF//////////////////////////////////////////////////////////////////
Enemy::Enemy(Stat hp, Stat) : MaxHP(hp), HP(50), dmgDeal(2) {}
Enemy::Enemy() : MaxHP(50), HP(50), dmgDeal(2) {}
void Enemy::Attack(Player &a, Stat dmg)
{
    a.HP -= dmg;
    if (a.HP < 0)
        a.HP = 0;
}
void Enemy::Heal(Stat heals)
{
    HP += heals;
    if (HP > MaxHP)
        HP = MaxHP;
}
///////////////////////////////////////////////////////////////////////////////
///
// DEATH STUFF////////////////////////////////////////////////////////////////
/*void getDead(bool state) {
    return state;
}*/
bool globalDead;
bool getDead(){
    return globalDead;
}
int DeadHelp(int& which)
{
    if(get_key(XK_k)) {
        std::srand(std::time(NULL));
        which = std::rand() % 17;
    }
    return which;
}
void DeadCheck(bool& state, int xres, int yres, int which)
{
    std::vector<const char *> quips{
    "You are trying... right?",  
    "Consider Fortnite!", 
    "Consider Roblox!", 
    "erm", 
    "git gud...",
    "You suck.",
    "Consider the Talk-Tuah Podcast...", 
    "On skibidi ????", 
    "We've been trying to reach you concerning your car's extended warranty.",
    "*teleports behind you* nothing personal, kid...",
    "The Flat Earth Society has members all around the globe!",
    "yikes.",
    "How many fingers am I holding up?",
    "bad bad not good",
    "Try again!", 
    "Let's take a deep breath.",
    "You've got to be trolling."
    };
    Rect d, q, a, b;
    if (state) {
        globalDead = 1;
        glEnable(GL_TEXTURE_2D);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
        glBegin(GL_QUADS);
            glVertex2i(0, yres);
            glVertex2i(0, 0);
            glVertex2i(xres, 0);
            glVertex2i(xres, yres);
        glEnd();
        int i = which;
        //words on death screen 
        d.bot = yres/2;
        d.left = xres / 2 - 70;
        d.center = 0;
        ggprint16(&d, 16, 0x88B00000, "GAME OVER.");
        int wrd = strlen(quips[i]);
        q.bot = d.bot-10;
        q.left = xres / 2 - (wrd*3);
        q.center = 0;
        ggprint10(&q, 16, 0xFFCCCCBB, quips[i]);
        a.bot = q.bot-60;
        a.left = xres /2 -  228;
        a.center = 0;
        ggprint12(&a, 16, 0x000FF000, "Q to QUIT");
        b.bot = q.bot-60;
        b.left = xres/2 + 60;
        b.center = 0;
        ggprint12(&b, 16, 0x000FF000, "ENTER to CONTINUE");
        if (get_key(XK_Return)) {
            state = 0;
            globalDead=0;
        }
        if (get_key(XK_q))
            Termination::Terminate(); 
    }
/*
ggprint16(&d, 16, 0x000FF000, ); biggest
ggprint12(&d, 16, 0x000FF000, );
ggprint13(&d, 16, 0x000FF000, );
ggprint10(&d, 16, 0x000FF000, ); good for quips
ggprint08(&d, 16, 0x000FF000, );
ggprint07(&d, 16, 0x000FF000, );
ggprint06(&d, 16, 0x000FF000, );
gprint8b(&d, 16, 0x000FF000, ;
*/
}
///////////////////////////////////////////////////////////////////////////////
///
/// dark mode /////////////////////////////////////////////////////////////////
void darkMode(bool click, int xres, int yres)
{
     if (click) {
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glDisable(GL_TEXTURE_2D);
         glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
         glBegin(GL_QUADS);
            glVertex2i(0, yres);
            glVertex2i(0, 0);
            glVertex2i(xres, 0);
            glVertex2i(xres, yres);
         glEnd();
         glEnable(GL_TEXTURE_2D);
         glDisable(GL_BLEND);
     } else {
         glDisable(GL_BLEND);
     } 
};
//////////////////////////////////////////////////////////////////////////////
///
/// other ////////////////////////////////////////////////////////////////////
 /*void supaspeed()
{
    if (get_key(XK_v)) {
        movement_speed = 0.5f;
    }
} */
