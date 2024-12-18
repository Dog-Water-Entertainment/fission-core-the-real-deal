#include "SceneManager.h"

// Mykull Guillory

Scene::~Scene() {}

Scene * Scene::GetNextScene()
{
    return m_pNextScene;
}

SceneManager::SceneManager()
{
    m_pScene = nullptr;
}

SceneManager::~SceneManager()
{
    Release();
}

void SceneManager::ChangeScene(Scene* scene)
{
    if (m_pScene != nullptr) {
        m_pScene->Release();
        delete m_pScene;
    }

    m_pScene = scene;
    m_pScene->Init();
}

void SceneManager::Update()
{
    if(m_pScene->GetNextScene() != nullptr) {
        ChangeScene(m_pScene->GetNextScene());
    }
    if(m_pScene != nullptr)
        m_pScene->Update();
}

void SceneManager::Render()
{
    if(m_pScene != nullptr)
        m_pScene->Render();
}

void SceneManager::Release()
{
    if (m_pScene != nullptr) {
        m_pScene->Release();
        delete m_pScene;
        m_pScene = nullptr;
    }
}

Scene * SceneManager::GetCurrentScene()
{
    return m_pScene;
}