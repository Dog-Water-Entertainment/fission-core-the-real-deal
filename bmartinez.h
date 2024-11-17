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
    int duration;     //duration of the effect (if buff/debuff)
    Item(int ID);
    */
//    ItemList item;
 //   int quantity;

    void Use(ItemList item, Player& a);
    void Use(ItemList item, Enemy& a);
    void Equip(ItemList item, Player& a);
    void Unequip(Player& a);

//ill figure a system out eventually
};
// get the logic for the holding and getEquippedItem stuff here and in 
// cpp file too
class Player
{
    public:
        Stat MaxHP;
        Stat HP;
        Stat baseDmg;  
        Stat dmgDeal;  
        ItemList Holding;
        std::vector<ItemList> Inventory;
        Player(Stat hp, Stat dmg);
        Player();
        void Attack(Enemy& a, Stat dmg);
        void AddItem(Player& a, ItemList item);
        void RemoveItem(Player& a, int spot); // 0-9 for spot.
       // void Heal (Stat heals);
        ItemList getEquippedItem();
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
        void Attack(Player &a, Stat dmg, bool& dead);
        //void Heal(Stat heals);
        //friend void Item::Use(int ID, Enemy& a); 
        friend void Item::Use(ItemList item, Player&a); 
};



void darkMode(bool click, int xres, int yres);
void DeadCheck(bool& state, int xres, int yres, int which);
int DeadHelp(int& which);
bool getDead();
//void supaspeed();
#endif
