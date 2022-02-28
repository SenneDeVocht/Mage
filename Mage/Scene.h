#pragma once
#include "SceneManager.h"

class GameObject;
class Scene
{
	friend Scene& SceneManager::CreateScene(const std::string& name);
public:
	void Add(std::unique_ptr<GameObject> object);

	void Update();
	void FixedUpdate();
	void DestroyMarkedObjects();
	void Render() const;

	std::vector<GameObject*> GetObjects() const;

	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private: 
	explicit Scene(const std::string& name);

	std::string m_Name;
	std::vector<std::unique_ptr<GameObject>> m_Objects{};

	static unsigned int m_IdCounter;
};