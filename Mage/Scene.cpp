#include "MagePCH.h"
#include "Scene.h"
#include "GameObject.h"

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
}

std::vector<GameObject*> Scene::GetObjects() const
{
	std::vector<GameObject*> vecRaw;

	vecRaw.reserve(m_Objects.size());
	std::transform(m_Objects.cbegin(), m_Objects.cend(), std::back_inserter(vecRaw),
		[](auto& ptr) { return ptr.get(); });

	return vecRaw;
}


void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		object->FixedUpdate();
	}
}

void Scene::DestroyMarkedObjects()
{
	// Destroy marked root objects
	const auto pos = std::remove_if(m_Objects.begin(), m_Objects.end(),
		[](const auto& o) { return o->IsMarkedForDestroy(); });

	m_Objects.erase(pos, m_Objects.end());

	// Call on remaining objects
	for (auto& object : m_Objects)
	{
		object->DestroyMarkedObjects();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

