#include "MagePCH.h"
#include "Scene.h"
#include "GameObject.h"

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name(name)
{
}

const std::string& Scene::GetName() const
{
	return m_Name;
}

GameObject* Scene::CreateObject(const std::string& name)
{
	auto object = std::unique_ptr<GameObject>(new GameObject(name, nullptr));
	const auto pObject = object.get();

	m_Objects.push_back(std::move(object));

	return pObject;
}

std::vector<GameObject*> Scene::GetObjects() const
{
	std::vector<GameObject*> vecRaw;

	vecRaw.reserve(m_Objects.size());
	std::transform(m_Objects.cbegin(), m_Objects.cend(), std::back_inserter(vecRaw),
		[](auto& ptr) { return ptr.get(); });

	return vecRaw;
}

void Scene::DrawImGui() const
{
	for (const auto& object : m_Objects)
	{
		object->DrawImGui();
	}
}

void Scene::Update() const
{
	for(const auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::FixedUpdate() const
{
	for (const auto& object : m_Objects)
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
	for (const auto& object : m_Objects)
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

