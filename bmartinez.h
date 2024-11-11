//hello hello
//headers
#ifndef BMARTINEZ_H
#define BMARTINEZ_H
typedef int Stat;
//forward declarations
class Enemy;

class Player
{
    private:
        Stat MaxHP;

    public:
        Stat HP;
        Stat dmgDeal;  
        Player(Stat hp, Stat dmg);
        Player();
        void Attack(Enemy &a, Stat dmg);
        void Heal (Stat heals); 
};

class Enemy
{
    private:
        Stat MaxHP;

    public:
        Stat HP;
        Stat dmgDeal;
        Enemy(Stat hp, Stat dmg);
        Enemy();
        void Attack(Player &a, Stat dmg);
        void Heal(Stat heals);
};

class Item
{
    public:
    int itemID;       // 1 = heals, 2 = super heals, 3 = buff (+10 max health 
                      // temporarily) 4 = minor debuff (-10 max health) 
                      // idk just ideas.
    int duration;     //duration of the effect (if buff/debuff)
    Item(int ID);

//ill figure a system out eventually
};


void darkMode(bool click, int xres, int yres);
void DeadCheck(bool state, int xres, int yres, int which);
int DeadHelp(int& which);

#endif
