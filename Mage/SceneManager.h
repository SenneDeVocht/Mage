#pragma once
#include "Singleton.h"

class Scene;
class GameObject;

class SceneManager final : public Singleton<SceneManager>
{
public:
	Scene& CreateScene(const std::string& name);

	void Update();
	void FixedUpdate();
	void Render();

private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	std::vector<std::shared_ptr<Scene>> m_Scenes;

	void DisplaySceneGraph() const;
	void DisplayChildObjects(const GameObject* parent) const;
};