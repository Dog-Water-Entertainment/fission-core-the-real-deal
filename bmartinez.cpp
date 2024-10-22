//author: brian martinez
//original start date: ?
//current date: 10/22/24
#include <GL/glx.h>
#include <iostream>
#include <vector>
#include <string>
#include "bmartinez.h"
typedef int Stat;
// Forward Declarations
class Player;
class Enemy;
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
//PLAYER STUFF/////////////////////////////////////////////////////////////////
Player::Player(Stat hp, Stat dmg) : MaxHP(hp), HP(hp), dmgDeal(dmg) {}
Player::Player() : MaxHP(50), HP(50), dmgDeal(2) {}

void Player::Attack(Enemy &a, Stat dmg)
{
    a.HP -= dmg;
    if (a.HP < 0)
        a.HP = 0;
}
void Player::Heal () 
{
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
void Enemy::Heal()
{
        
}
///////////////////////////////////////////////////////////////////////////////
///
// ITEM STUFF//////////////////////////////////////////////////////////////////
Item::Item(int ID) : itemID(ID) 
{
    std::cout << "placeholder" << ID  << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
///
// DEATH STUFF////////////////////////////////////////////////////////////////
void youDied()
{
    std::vector<std::string> quips{
    "You are trying, right?",  
    "Consider Fortnite!", 
    "Consider Roblox!", 
    "Us: 99999, you: 0", 
    "git gud...",
    "You suck.",
    "Consider the Talk-Tuah Podcast...", 
    "on skibidi ????", 
    "We've been trying to reach you concerning your vehicle's extended warranty.",
    "*teleports behind you* nothing personal, kid...", 
    "What the flip did you just flipping say about me, you little rascal? "
    "I'll have you know I graduated top of my class in the Navy Seals, and "
    "I've been involved in numerous secret raids on Al-Quaeda, and I have over"
    " 300 confirmed kills. I am trained in gorilla warfare and I'm the top "
    "sniper in the entire US armed forces. You are nothing to me but just "
    "another target. I will wipe you the flip out with precision the likes of "
    "which has never been seen before on this Earth, mark my flipping words. "
    "You think you can get away with saying that crap to me over the Internet? "
    "Think again, little guy. As we speak I am contacting my secret network of "
    "spies across the USA and your IP is being traced right now so you better "
    "prepare for the storm, maggot. The storm that wipes out the pathetic "
    "little thing you call your life. You're flippin' dead, kid. I can be "
    "anywhere, anytime, and I can kill you in over seven hundred ways, and "
    "that's just with my bare hands. Not only am I extensively trained in "
    "unarmed combat, but I have access to the entire arsenal of the United "
    "States Marine Corps and I will use it to its full extent to wipe your "
    "miserable cheeks off the face of the continent, you little poop. If only "
    "you could have known what unholy retribution your little \"clever\" "
    "comment was about to bring down upon you, maybe you would have held your "
    "flipping tongue. But you couldn't, you didn't, and now you're paying the "
    "price, you goshdarn nincompoop. I will poo fury all over you and you will "
    "drown in it.You're frinkin dead, kiddo."
    };
    //int number = getrandomnumber(0, sizeofvector) 
    //std::cout << quips[number] << endl;
    // its supposed to write to the screen but ill figure that out once 
    // the scene loader can be used or something idk, pick a random number
    // spit out the line at that index
}

