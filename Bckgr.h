#ifndef BCKGR_H
#define BCKGR_H

class Bckgr
{
    private:
        static Bckgr* instance;
        Bckgr() {}

    public:
        static Bckgr* getInstance();
        void dialoguebackground();
};
#endif
