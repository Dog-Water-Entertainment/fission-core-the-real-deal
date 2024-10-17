class Scene 
{
public:
    Scene();
    virtual ~Scene();

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Release() = 0;
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
};

