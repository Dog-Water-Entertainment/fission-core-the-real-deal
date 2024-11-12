#ifndef _BOSS_SCENE_H_
#define _BOSS_SCENE_H_

#include "SceneManager.h"
#include "Inputs.h"
#include "Shapes.h"
#include <GL/glx.h>

class BossScene : public Scene
{
    private:
        GLuint texture;

        GLuint playerTexture;
        GLuint silhouetteTexture;
        float anglePlayerLook;
        Rectangle m_rectangle;
    public:
        BossScene(int xres, int yres);
        ~BossScene();

        void Init();
        void Update();
        void Render();
        void Release();
};

#endif