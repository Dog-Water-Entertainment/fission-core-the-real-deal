#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H


// Virtual class for scenes
// If we want to create a new scene, 
// we need to inherit from this class
// and implement the virtual functions
class Scene 
{
protected:
    int m_xres;
    int m_yres;
    Scene * m_pNextScene;
public:
    virtual ~Scene();


    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Release() = 0;
    Scene * GetNextScene();
};

class SceneManager
{
private:
    Scene* m_pScene;
public:
    SceneManager();
    ~SceneManager();

    void ChangeScene(Scene* scene);
    void Update();
    void Render();
    void Release();
    Scene * GetCurrentScene();
};

#endif // SCENE_MANAGER_H