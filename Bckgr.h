#ifndef BCKGR_H
#define BCKGR_H

class Bckgr
{
    private:
        static Bckgr* instance;
        
    public:
        static Bckgr* getInstance();
        static void dialoguebackground();
};
#endif
