//hello hello
//headers
#ifndef BMARTINEZ_H
#define BMARTINEZ_H
typedef int Stat;
//forward declarations

class Enemy;
class Player;
class Item;
enum class ItemList;

class Item
{
    public:
   /* int itemID;       // 1 = heals, 2 = super heals, 3 = buff (+10 max health 
                      // temporarily) 4 = minor debuff (-10 max health) 
                      // idk just ideas.
    int duration;     //duration of the effect (if buff/debuff)
    Item(int ID);
    */
    void Use(ItemList item, Player& a);
    void Use(ItemList item, Enemy& a);

//ill figure a system out eventually
};

class Player
{
    public:
        Stat MaxHP;
        Stat HP;
        Stat dmgDeal;  
        Player(Stat hp, Stat dmg);
        Player();
        void Attack(Enemy &a, Stat dmg);
        void Heal (Stat heals);
        //friend void Item::Use(int ID, Player& a); 
        friend void Item::Use(ItemList item, Player&a); 
};

class Enemy
{
    public:
        Stat MaxHP;
        Stat HP;
        Stat dmgDeal;
        Enemy(Stat hp, Stat dmg);
        Enemy();
        void Attack(Player &a, Stat dmg);
        void Heal(Stat heals);
        //friend void Item::Use(int ID, Enemy& a); 
        friend void Item::Use(ItemList item, Player&a); 
};



void darkMode(bool click, int xres, int yres);
void DeadCheck(bool& state, int xres, int yres, int which);
int DeadHelp(int& which);
bool getDead();
//void supaspeed();
#endif
