#ifndef TitleScreenScene_H
#define TitleScreenScene_H

#include "../Scene.h"

class TitleScreenScene : public Scene
{
    public:
        TitleScreenScene(int xres, int yres);
        ~TitleScreenScene();

        void init();
        void update();
        void render();
        void release();
}

#endif // TitleScreenScene_H