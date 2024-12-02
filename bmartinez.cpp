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
// - attack function could use the Stat variable for dmg to deal damage instead 
// of passing it like a pareameter but that could be a pain in the ass, we'll
// see
// - Finish battle loop
// - get health bars / health information to render during boss fights
// - use elias' thing to retrieve what selection youve chosen during the 
//   attack/potion/flee/whatever 
// - get enemy "AI" done.
// - undertale style textbox render
// - get the hitbox thing made
// X get the map made
// - get the scene to switch upon touching hitbox
// - get prompted to open chest and get sword/ potions 
// - dialogue saying you got the item
// - dev weapon cheatcode
//
// - dark souls boss vanquished scene 
//---------------------------------------------------------------------------
// Forward Declarations////////////////////////////////////////////////////////
class Bckgr;
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
                //no item what r u doing
                std::cout << "not recognized item" << std::endl;
        }
    } else {
        std::cout << "the player doesnt have that item to use, creature" << 
            std::endl;
    }
}
void Item::Use(ItemList item, Enemy& a) //no inventory, this is just a 
                                        // healing for them
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
            std::cout << "u cant equip that" << std::endl;
    }

}

void Item::Unequip(Player& a)
{
    if (a.getEquippedItem() == ItemList::Empty) {
        std::cout << "theres nothing to unequip monkey" << std::endl; // change
                                             // to use dialogue stuff later
        return;
    }
    a.Holding = ItemList::Empty;
    a.dmgDeal = a.baseDmg;
}
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

void Player::Attack(Enemy &a, Stat dmg) //always pass dmgDeal. not baseDmg
                                        //unles u wanna get silly then 
                                        //pass whatever u want
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
                std::cout << "unequip first dork" << std::endl;
                return;
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
void Enemy::Attack(Player &a, Stat dmg, bool& dead) // pass dead so that it can
                                                    // be triggered`
{
    a.HP -= dmg;
    if (a.HP < 0) {
        a.HP = 0;
        dead = 1;
    }
}
///////////////////////////////////////////////////////////////////////////////
///
//BATTLE///////////////////////////////////////////////////////////////////////
/*
void Battle::BattleStart(Player& a, Enemy& b)
{
    this->player = &a;
    this->enemy = &b;
    inBattle = 1;
    playerDead = 0;
    beatEnemy = 0;
    madeChoice = 0;
    playerChoice = -1;

}
void Battle::Update(float deltaTime)
{
    if (!inBattle)
        return;
    if (!madeChoice)
        //get the thing to render
        //process the input somehow
    bool fleed = 0
        switch (playerChoice) 
           // case fight:
        //       (play animation);
                Player::Attack(b, a.dmgDeal);
                if (b.HP == 0) 
                    beatEnemy = 1;
                break;
           // case flee:
           //     (play animation);
                bool erm = flee(); // 1/100 chance of fleeing maybe ? idk,its 
                if (erm) {
                    fleed = 1;
                    break;  // cuz the only fight in this demo is the boss
                } else {
                    //no no no tsk tsk tsk
                    break;
                }
               case heal:
                   Item::Use(HealingPotion, this->player)
                   dialogue saying u healed or something idk 
                   break;
            default:
                std::cerr << "something went wrong, try again" << std::endl;
         }
        
         if (beatEnemy) {
             beatEnemy(); 
        // dialogue maybe ? ()
        }

        enemySide(a, b, dead); //check if it kills you in the while loop 
                               //( not in the function for readability's sake )
        // maybe ill write a functino here that forces everythin to stop until
        // you make a choice 
        // back up to top of while
        if (player
    }

*/
/*
void BattleStart(Player& a, Enemy& b, bool& dead)
{
    if (!inBattle)
        return;
    if (!madeChoice)
        //get the thing to render
        //process the input somehow
    bool fleed = 0
   @@@// while (dead == 0 || beatEnemy == 0) {
        //wait for choice somehow ? or prompt choice or maybe wait at the end 
        //
        //of the loop for choice to be made before we continue back to thispoint
        //
 @@@       //getChoice() 
     
        switch (playerChoice) 
           // case fight:
        //       (play animation);
                Player::Attack(b, a.dmgDeal);
                if (b.HP == 0) 
                    beatEnemy = 1;
                break;
           // case flee:
           //     (play animation);
                bool erm = flee(); // 1/100 chance of fleeing maybe ? idk,its 
                if (erm) {
                    fleed = 1;
                    break;  // cuz the only fight in this demo is the boss
                } else {
                    //no no no tsk tsk tsk
                    break;
                }
         @ //  case item:
         @      // ItemList::chosen = itemScreen();
         @     // a.Use(chosen); |i think thats how i set it up lmfao, hopefully
         @     // dialgoue for success| it deletes the item from the thing afte
               case heal:
                   Item::Use(HealingPotion, this->player)
                   dialogue saying u healed or something idk 
                   break;
            default:
                std::cerr << "something went wrong, try again" << std::endl;
         }
        
         if (beatEnemy) {
             beatEnemy(); 
        // dialogue maybe ? ()
        }

        enemySide(a, b, dead); //check if it kills you in the while loop 
                               //( not in the function for readability's sake )
        // maybe ill write a functino here that forces everythin to stop until
        // you make a choice 
        // back up to top of while
        if (player
    }


}

Battle::Battle() : inBattle(0), madeChoice(0) {}

static Battle& Battle::getInstance()
{
    static Battle instance;
    return instance;
}
*/
/*
void switcher(); // presses T for you lmfao
void enemySide(Player& a, Enemy& b, bool& dead)
{
    //random number that chooses attack or heal
    srand(time(NULL));
    int randNum = (rand() % 2) + 1;
    int maxHeals = 3;


    switch (randNum)
    {
        case 1:
            Enemy::Attack(a, b.dmgDeal, dead);
            break;
        case 2:
            Item::Use(
            break;
        default:
            //silly
            break;
    }
    return;
}
int getChoice();
void shakeAnimation(); //maybe ??? ? ? ? ?? 
bool flee()
{
    //srand stuff 
}
void beatEnemy()
{
   //     breaks out of the battle scene somehow and prints boss killed
}
*/
///////////////////////////////////////////////////////////////////////////////
///
// DEATH STUFF////////////////////////////////////////////////////////////////
bool globalDead;  
bool getDead()
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
         //   state = 0;
          //  globalDead=0;
          
          
            char* argv[] = {const_cast<char*>("./fissioncore"), nullptr};
            execvp(argv[0], argv);

            perror("failed to restart");
            exit(EXIT_FAILURE);
           /* state = 0;
            globalDead = 0;
            m_pNextScene = new MapScreen(m_xres, m_yres);
          */
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
/// dialogue background //////////////////////////////////////////////////////
Bckgr* Bckgr::getInstance()
{
    if (instance == nullptr)
        instance = new Bckgr();
    return instance;
}
Bckgr* Bckgr::instance = nullptr;

//void Bckgr::dialoguebackground(bool& speaking)
void Bckgr::dialoguebackground()
{
    bool speaking = 1;
    if (speaking) {
    //  if (DialogManager::isDialogActive()) {
        glDisable(GL_TEXTURE_2D);
        glColor3ub(255, 255, 255);
        glBegin(GL_QUADS);
            glVertex2i(15, 155);  //topleft
            glVertex2i(15, 15);
            glVertex2i(785, 15);
            glVertex2i(785, 155); //top right  (counter clockwise)
        glEnd();

        glColor3ub(0, 0, 0);
        glBegin(GL_QUADS);
            glVertex2i(20, 150);  //topleft
            glVertex2i(20, 20);
            glVertex2i(780, 20);
            glVertex2i(780, 150); //top right  (counter clockwise)
        glEnd(); 
        glEnable(GL_TEXTURE_2D);
        //if (get_key(XK_Return)) {
	    if (DialogManager::isDialogActive() == 0) {
            speaking = 0;
            return;
        }
    }
}

