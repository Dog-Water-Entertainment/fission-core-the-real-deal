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
    cout << "status: " << click << endl;
     if (click) {
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
         glBlendFunc(GL_TEXTURE_2D, GL_ONE_MINUS_DST_ALPHA);
         glColor4f(0, 0, 0, 0.3);
         glBegin(GL_QUADS);
         glVertex2i(0, yres);
         glVertex2i(0, 0);
         glVertex2i(xres, 0);
         glVertex2i(xres, yres);
         glEnd();
         //glBlendFunc(GL_ONE, GL_ZERO);
         glDisable(GL_BLEND);
     }
};


