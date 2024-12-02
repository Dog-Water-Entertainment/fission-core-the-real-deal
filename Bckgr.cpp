//author: brian martinez
//purpose: get the background to load with the dialogue
//12/2/24
#include <GL/glx.h>
#include "Bckgr.h"
#include "efarmer.h"

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

