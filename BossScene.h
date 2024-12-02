#ifndef _BOSS_SCENE_H_
#define _BOSS_SCENE_H_

#include "SceneManager.h"
#include "Inputs.h"
#include "Shapes.h"
#include "bmartinez.h"
#include "efarmer.h"
#include <GL/glx.h>

// Origin: Mykull Guillory

class BossScene : public Scene
{
    private:
        Player player;
        Enemy enemy;
        float anglePlayerLook;
        Rectangle m_rectangle;
        Vec2 oldPlayerPos;
        bool fighting;
        bool * dead;
        bool isFighting();

        Line degubLine;
    public:
        BossScene(int xres, int yres);
        ~BossScene();

        void Init();
        void Update();
        void Render();
        void Release();

        void setDead(bool * dead);

        void setPlayerPos(float x, float y);
        void exitScene();

};

#endif