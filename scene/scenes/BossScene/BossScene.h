#ifndef _BOSS_SCENE_H_
#define _BOSS_SCENE_H_

#include "../../SceneManager.h"
#include "../../../inputs/Inputs.h"


class BossScene : public Scene
{
    public:
        BossScene(int xres, int yres);
        ~BossScene();

        void Init();
        void Update();
        void Render();
        void Release();
};

#endif