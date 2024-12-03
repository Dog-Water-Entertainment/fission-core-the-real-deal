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
#include <unistd.h>
#include "bmartinez.h"
#include "Inputs.h"
#include "fonts.h"
#include "efarmer.h"
#include "MapScreen.h"
#include "BossScene.h"

//TODO:----------------------------------------------------------------------- 
// X probably an inventory system should be ezpz, just an vector of item
// x get enemy "AI" done.
// X get the map made
//---------------------------------------------------------------------------
// Forward Declarations////////////////////////////////////////////////////////
//class Bckgr;
class Item;
class Player;
class Enemy;
///////////////////////////////////////////////////////////////////////////////
///
// ITEM STUFF//////////////////////////////////////////////////////////////////
///note: the cout's in alot of these are not for the player but for us to debug
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
std::string item2str(ItemList item)
{
    switch (item) {
        case ItemList::Empty:
            return "";
        case ItemList::HealingPotion:
            return "Healing Potion";
        case ItemList::GreaterHealingPotion:
            return "Greater Healing Potion";
        case ItemList::Knife:
            return "Knife";
        case ItemList::Sword:
            return "Sword";
        default:
            std::cout << "not recognized item" << std::endl;
            return "";
    }
}
void Item::Use(ItemList item, Player& a)
{
    bool have = 0;
    for (size_t i = 0; i < a.Inventory.size(); i++) {
        if (item == a.Inventory[i])
                have = 1;
    }
    if (have == 1) {
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
                std::cout << "not recognized item" << std::endl;
        }
    } else {
        std::cout << "the player doesnt have that item" << std::endl;
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
            std::cout << "not recognized item" << std::endl;
    }
}

void Item::Equip(ItemList item, Player& a)
{
    if (a.getEquippedItem() != ItemList::Empty) { //unequips before equipping
        a.Holding = ItemList::Empty;
        a.dmgDeal = a.baseDmg;
    }

    switch (item) {             //write check so that items cant be equipped 
                                //multiple times to get +10+10+10 etc
        case ItemList::Knife:
            if (a.dmgDeal >= a.baseDmg +8) {
                DialogManager::promptDialog("Player", 
                    {"I can't hold both of these things in the same hand..."}, 
                    800 / 2 - 300, 100, 0x00ffffff);
                break;
            }
            a.dmgDeal += 8;
            a.Holding = ItemList::Knife;
            break;
        case ItemList::Sword:
            if (a.dmgDeal >= a.baseDmg +15) {
                DialogManager::promptDialog("Player", 
                        {"I can't hold both of these things in the same hand..."},
                        800 / 2 - 300, 100, 0x00ffffff);
                break;
            }
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
            std::cout << "can not equip" << std::endl;
    }

}

void Item::Unequip(Player& a)
{
    if (a.getEquippedItem() == ItemList::Empty) {
        DialogManager::promptDialog("Player", 
                {"Nothing there."}, 800 / 2 - 300, 100, 0x00ffffff);
        return;
    }
    a.Holding = ItemList::Empty;
    a.dmgDeal = a.baseDmg;
}
///////////////////////////////////////////////////////////////////////////////
///
//PLAYER STUFF/////////////////////////////////////////////////////////////////
Player::Player(Stat hp, Stat dmg) : MaxHP(hp), HP(hp), baseDmg(dmg), 
    dmgDeal(dmg), healAmt(4), currentAmt(0), Holding(ItemList::Empty) 
{
    Inventory = std::vector<ItemList>(10, ItemList::Empty);
} 
Player::Player() : MaxHP(100), HP(100), baseDmg(10), dmgDeal(10), healAmt(4),
    currentAmt(0), Holding(ItemList::Empty) 
{
    Inventory = std::vector<ItemList>(10, ItemList::Empty);
}

void Player::Attack(Enemy &a, Stat dmg) //always pass dmgDeal. not baseDmg
{
    a.HP -= dmg;
    if (a.HP < 0)
        a.HP = 0;
}

void Player::Heal(Stat heals)
{
    if (healAmt > currentAmt) {
        if (HP >= MaxHP) {
            DialogManager::promptDialog("Player", 
                    {"Already at maximum health capacity."}, 
                    800 / 2 - 300, 100, 0x00ffffff);
        }
        HP += heals; 
        if (MaxHP < HP)
            HP = MaxHP;
        currentAmt += 1;
    } else {
        DialogManager::promptDialog("Player", 
                {"Out of potions..."}, 
                800 / 2 - 300, 100, 0x00ffffff);
    }
}

ItemList Player::getEquippedItem() //can be used even if nothing is equipped 
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
}

void RemoveItem(Player& a, ItemList item)       
{   
    for (size_t i = 0; i < a.Inventory.size(); i++)
    {
        if (a.Inventory[i] == item) {  //WIP, CHECK IF ITS EQUIPPED FIRST
            if (a.getEquippedItem() == item) {
                DialogManager::promptDialog("Player", 
                    {"Can't remove equipped item."}, 
                    800 / 2 - 300, 100, 0x00ffffff);
                return;
            }
            a.Inventory[i] = ItemList::Empty;
            return;
        }
    }
    DialogManager::promptDialog("Player", {"Nothing to remove..."}, 
            800 / 2 - 300, 100, 0x00ffffff);
}
void showInv()
{
    if (get_key(XK_i)) {
        DialogManager::promptDialog("Terrance", {"Empty pockets..."}, 
                800 / 2 - 300, 100, 0x00ffffff);
    }
   /* 
void showInv(Player& a)
{
    if (get_key(XK_i)) {
        std::string output = "Items currently in your inventory: ";
        bool first = 1;
        for (int i = 0; i < 10; i++) {
            std::string temp = item2str(a.Inventory[i]);
            if (!temp.empty()){
                if (!first)
                    output += ", ";
                output += temp;
                first = 0;
            }
        }
        DialogManager::promptDialog("Terrance", {output}, 
                800 / 2 - 300, 100, 0xfff00000);
    }
  */
}

///////////////////////////////////////////////////////////////////////////////
///
//ENEMY STUFF//////////////////////////////////////////////////////////////////
Enemy::Enemy(Stat hp, Stat dmgIn) : MaxHP(hp), HP(hp), dmgDeal(dmgIn) {}
Enemy::Enemy() : MaxHP(50), HP(50), dmgDeal(8) {}
void Enemy::Attack(Player &a, Stat dmg, bool& dead) // pass dead so that it can
{
    a.HP -= dmg;
    if (a.HP <= 0) {
        a.HP = 0;
        dead = 1;
    }
}
void Enemy::Heal(Stat heals)
{
    HP += heals;
    if (HP > MaxHP)
        HP = MaxHP;
    return;
}
void Enemy::enemySide(Player& a, Enemy& b, bool& dead)
{
    //random number that chooses attack or heal
    srand(time(NULL));
    int randNum = (rand() % 2) + 1;
    int maxHeals = 3;
    int currentHeals = 0;
    switch (randNum)
    {
        case 1:
            b.Attack(a, b.dmgDeal, dead);
            if (!dead)
                DialogManager::promptDialog("Enemy", {"..."}, 
                800 / 2 - 300, 100, 0xfff00000);
            break;
        case 2:
            if (currentHeals < maxHeals) {
                b.Heal(20);
                currentHeals++;
                DialogManager::promptDialog("Enemy", 
                    {"Shouldn't have let me heal."}, 
                    800 / 2 - 300, 100, 0xfff00000);
            } else {
                b.Attack(a, b.dmgDeal, dead);
            if (!dead)
                DialogManager::promptDialog("Enemy", {"..."}, 
                800 / 2 - 300, 100, 0xfff00000);
            }
            break;
        default:
            std::cout << "something broke" << std::endl;
            break;
    }
    return;
}
///////////////////////////////////////////////////////////////////////////////
///
// DEATH STUFF////////////////////////////////////////////////////////////////
bool globalDead;  

bool Death::getDead()
{
    return globalDead;
}

int DeadHelp(int& which)
{
    if (get_key(XK_k)) {
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
        ggprint12(&b, 16, 0x000FF000, "ENTER to RESTART");
        if (get_key(XK_Return)) {
            char* argv[] = {const_cast<char*>("./fissioncore"), nullptr};
            execvp(argv[0], argv);

            perror("failed to restart");
            exit(EXIT_FAILURE);
        }
        if (get_key(XK_q))
            Termination::Terminate(); 
    }
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
///extra//////////////////////////////////////////////////////////////////////
void story(bool& first)
{
    if (first) {
        DialogManager::promptDialog("Player", {"...", "argh", 
                "What's going on? Why's my head pounding?", 
                "What happened to this place?", "Something moved.", 
                "...is that thing even human?", "I have to find a way out."}, 
                800 / 2 - 300, 100, 0x00ffffff);
    first = 0;
    }
}

