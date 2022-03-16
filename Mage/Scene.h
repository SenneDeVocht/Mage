#pragma once
#include "SceneManager.h"

class GameObject;
class Scene final
{
	friend Scene* SceneManager::CreateScene(const std::string& name);

public:
	explicit Scene(const std::string& name);
	const std::string& GetName() const;

	void DrawImGui() const;
	void Update() const;
	void FixedUpdate() const;
	void DestroyMarkedObjects();
	void Render() const;

	GameObject* CreateObject(const std::string& name);
	std::vector<GameObject*> GetObjects() const;

private:
	std::string m_Name;
	std::vector<std::unique_ptr<GameObject>> m_Objects{};

	static unsigned int m_IdCounter;
};