//brian martinez source file.
//
//
//
//
//
#include <GL/glx.h>
#include <iostream>
using namespace std;
void lastStand(bool click, int xres, int yres)
{
     if (click) {
         //glClear(GL_COLOR_BUFFER_BIT);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        // glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glDisable(GL_TEXTURE_2D);
         glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
         glBegin(GL_QUADS);
            glVertex2i(0, yres);
            glVertex2i(0, 0);
            glVertex2i(xres, 0);
            glVertex2i(xres, yres);
         glEnd();
         glEnable(GL_TEXTURE_2D);
         //glDisable(GL_BLEND);
         //glBlendFunc(GL_ONE, GL_ZERO);
         glDisable(GL_BLEND);
     } else {

         //glBlendFunc(GL_ONE, GL_ZERO);
         glDisable(GL_BLEND);
     } 
};


