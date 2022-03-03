#pragma once
#include "Singleton.h"

class Scene;
class GameObject;

class SceneManager final : public Singleton<SceneManager>
{
public:
	~SceneManager() override;

	Scene* CreateScene(const std::string& name);

	void Update();
	void FixedUpdate();
	void DestroyMarkedObjects();
	void Render();

private:
	friend class Singleton<SceneManager>;
	SceneManager();

	void DisplaySceneGraph() const;
	void DisplayChildObjects(const GameObject* parent) const;

	std::vector<std::unique_ptr<Scene>> m_Scenes;
};