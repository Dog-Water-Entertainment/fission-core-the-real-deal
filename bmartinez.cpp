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

//TODO: probably an inventory system should be ezpz, just an vector of item
//
// Forward Declarations////////////////////////////////////////////////////////
class Item;
class Player;
class Enemy;
///////////////////////////////////////////////////////////////////////////////
///
// ITEM STUFF//////////////////////////////////////////////////////////////////
enum class ItemList {
    Empty = 0,
    HealingPotion,          //20 hp heal
    GreaterHealingPotion,       // rarer, 50 hp heal
    PermaHealth,                // terraria life crystal, +10 max hp
    Knife,                      // dmg boost, +8dmg
    Sword,                      // dmg boost, +15 dmg 
    PermaDmg,                   // dmg boost, +5 to base dmg
    DevWeapon,                  // for us: 9999999 dmg
    DevPotion                   // for us: 9999999 hp
};

void Item::Use(ItemList item, Player& a)
{
    switch (item) {
        case ItemList::HealingPotion:
            a.HP += 20; 
            if (a.MaxHP < a.HP)
               a.HP = a.MaxHP; 
            break;
        case ItemList::GreaterHealingPotion:
            a.HP += 50;
            if (a.MaxHP < a.HP)
                a.HP = a.MaxHP;
            break;
        case ItemList::PermaHealth:
            a.MaxHP += 10;
            break;
        case ItemList::PermaDmg:
            a.baseDmg += 5;
            break;
        case ItemList::DevPotion:
            if (a.MaxHP == 999999) {
                a.HP = 999999;
                break;
            }
            a.MaxHP = 999999;
            a.HP = 999999;
            break;
        default:
            //no item what r u doing
            std::cout << "ok" << std::endl;
    }
}

void Item::Use(ItemList item, Enemy& a)
{
    switch (item) {
        case ItemList::HealingPotion:
            a.HP += 15;
            if (a.MaxHP < a.HP)
               a.HP = a.MaxHP; 
            break;
        case ItemList::GreaterHealingPotion:
            a.HP += 50;
            if (a.MaxHP < a.HP)
                a.HP = a.MaxHP;
            break;
        default:
            //no item what r u doing
            std::cout << "ok" << std::endl;
    }
}

void Item::Equip(ItemList item, Player& a)
{
    /*if (a.baseDmg < a.dmgDeal)   //is something already equipped?
        a.dmgDeal = a.baseDmg;   // unequip
      */                           // might add a bool to player class that 
    if (a.getEquippedItem() != ItemList::Empty) {
        a.Holding = ItemList::Empty;
        a.dmgDeal = a.baseDmg;
    }

    switch (item) {             //write check so that items cant be equipped 
                                //multiple times to get +10+10+10 etc
        case ItemList::Knife:
            if (a.dmgDeal >= a.baseDmg +8)
                //msg that says nonono not good little fella
                break;
            a.dmgDeal += 8;
            a.Holding = ItemList::Knife;
            break;
        case ItemList::Sword:
            if (a.dmgDeal >= a.baseDmg +15)
                //msg that says nonono not good little fella
                break;
            a.dmgDeal +=15;
            a.Holding = ItemList::Sword;
            break;
        case ItemList::DevWeapon:
            if (a.dmgDeal == 999999)
                break;
            a.baseDmg = 999999;
            a.dmgDeal = 999999;
            a.Holding = ItemList::DevWeapon;
            break;
        default: 
            std::cout << "not good litte fella" << std::endl; //placeholder
    }

}
void Item::Unequip(Player& a)
{
    a.Holding = ItemList::Empty;
    a.dmgDeal = a.baseDmg;
}

/* void Item::Equip(ItemList item, Enemy& a)
{
    switch (item) {
        case ItemList::Knife:
            if ()
               abc;
            break;
        case ItemList::
            if ()
               abc;
            break;
        case ItemList::
            if ()
               abc;
            break;
        case ItemList::
            if ()
               abc;
            break;
        case ItemList::
            if ()
               abc;
            break;
        default: 
            std::cout << "not good litte fella" << std::endl; //placeholder
    }
}
*/
///////////////////////////////////////////////////////////////////////////////
///
//PLAYER STUFF/////////////////////////////////////////////////////////////////
Player::Player(Stat hp, Stat dmg) : MaxHP(hp), HP(hp), baseDmg(dmg), 
    dmgDeal(dmg){}
Player::Player() : MaxHP(100), HP(100), baseDmg(10), dmgDeal(10), 
    Holding(ItemList::Empty) 
{
    Inventory = std::vector<ItemList>(10, ItemList::Empty);
}

void Player::Attack(Enemy &a, Stat dmg)
{
    a.HP -= dmg;
    if (a.HP < 0)
        a.HP = 0;
}

ItemList Player::getEquippedItem() //can be used even if nothing is equipped 
                                   //(returns none)
{
    return Holding;

}

void Player::AddItem(Player& a, ItemList item)
{
    for (size_t i = 0; i < a.Inventory.size(); i++) {
        if (Inventory[i] == ItemList::Empty) {
            Inventory[i] = item;
            return;
        }
    }
    //some message saying the inv is full, use or delete something
}

void RemoveItem(Player& a, ItemList item)       
{   
    for (size_t i = 0; i < a.Inventory.size(); i++)
    {
        if (a.Inventory[i] == item) {  //WIP, CHECK IF ITS EQUIPPED FIRST
            if (a.getEquippedItem() == item) {
                //unequip the item weirdo
                std::cout << "not good" << std::endl;
            }
            a.Inventory[i] = ItemList::Empty;
            return;
        }
    }
    std::cout << "that things not even in ur inventory" << std::endl;
}
/*
void Player::Heal(Stat heals) 
{
    HP += heals;
    if (HP > MaxHP)
        HP = MaxHP;
}
*/
///////////////////////////////////////////////////////////////////////////////
///
//ENEMY STUFF//////////////////////////////////////////////////////////////////
Enemy::Enemy(Stat hp, Stat) : MaxHP(hp), HP(50), dmgDeal(2) {}
Enemy::Enemy() : MaxHP(50), HP(50), dmgDeal(8) {}
void Enemy::Attack(Player &a, Stat dmg, bool& dead) //pass dead so that it can be triggered`
{
    a.HP -= dmg;
    if (a.HP < 0) {
        a.HP = 0;
        dead = 1;
    }
}
/*
void Enemy::Heal(Stat heals)
{
    HP += heals;
    if (HP > MaxHP)
        HP = MaxHP;
}
///////////////////////////////////////////////////////////////////////////////
///
// DEATH STUFF////////////////////////////////////////////////////////////////
void getDead(bool state) {
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
