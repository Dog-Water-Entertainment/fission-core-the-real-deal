//hello hello
//headers
#ifndef BMARTINEZ_H
#define BMARTINEZ_H
typedef int Stat;
//forward declarations

class Enemy;
class Player;
class Battle;
class Item;
class Bckgr;
enum class ItemList;

class Item
{
    public:
        void Use(ItemList item, Player& a); //will check if you have it in inv 
                                            //first
        void Use(ItemList item, Enemy& a); //wont check you can use any potion
        void Equip(ItemList item, Player& a); // will check if you have it, if 
                                              // you have anything already 
                                              // equipped it will unequip and 
                                              // equip new thing 
        void Unequip(Player& a);       // will check if anything is equipped 
                                       // at all before it tries unequipping

};
/*
class Battle
{
    public:
        static Battle& getInstance();
        void BattleStart(Player& a, Enemy& b);
        void Update();
    private:
        Battle() : inBattle(0), madeChoice(0) {}


        Player* player = nullptr;
        Enemy* enemy = nullptr;
        bool inBattle;
        bool playerDead;
        bool beatEnemy;
        bool madeChoice;
        int playerChoice






}
*/
class Player
{
    public:
        Stat MaxHP;              
        Stat HP;                       
        Stat baseDmg;                //dmg dealt without any weapons
        Stat dmgDeal;                //used in attack function, can be base 
                                     //+ weapon dmg
        ItemList Holding;            // item list held, could be 
                                     // ItemList::None
        std::vector<ItemList> Inventory;  //when deleting or using item, 
                                          //switches index
                                          //that held that item to None
        Player(Stat hp, Stat dmg);     
        Player();
        void Attack(Enemy& a, Stat dmg); // subtracts dmg deal from enemy hp
        void AddItem(Player& a, ItemList item); // checks if item is in 
                                                // inventory first
        void RemoveItem(Player& a, ItemList item); //^
        ItemList getEquippedItem();    //returns whatever is equippped
        friend void Item::Use(ItemList item, Player&a);
};

class Enemy
{
    public:            //most everything same as player 
        Stat MaxHP;
        Stat HP;
        Stat dmgDeal;
        Enemy(Stat hp, Stat dmg);
        Enemy();
        void Attack(Player &a, Stat dmg, bool& dead); // when player health 
                                                      // reaches 0 dead is set 
                                                      // to 1.
        //void Heal(Stat heals);
        //friend void Item::Use(int ID, Enemy& a); 
        friend void Item::Use(ItemList item, Enemy& a); //no inventory so this
                                                        //doesnt check anything,
                                                        //if you pass 
                                                        //it a potion it'll work
};

class Bckgr
{
    private:
        static Bckgr* instance;
        Bckgr() {}

    public:
        static Bckgr* getInstance();
        void dialoguebackground();


};
//void dialoguebackground(bool& speaking);
void darkMode(bool click, int xres, int yres); //dim the lights 
void DeadCheck(bool& state, int xres, int yres, int which); // dead scene 
int DeadHelp(int& which); // helps choose which quip will be displayed
bool getDead(); // returns if youre dead or not
#endif
