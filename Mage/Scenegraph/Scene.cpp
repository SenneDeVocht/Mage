#include "Mage/MagePCH.h"
#include "Scene.h"

#include "Mage/Scenegraph/GameObject.h"

unsigned int Mage::Scene::m_IdCounter = 0;

Mage::Scene::Scene(const std::string& name)
	: m_Name(name)
{
}

const std::string& Mage::Scene::GetName() const
{
	return m_Name;
}

Mage::GameObject* Mage::Scene::CreateObject(const std::string& name)
{
	auto object = std::unique_ptr<GameObject>(new GameObject(name, nullptr));
	const auto pObject = object.get();

	m_Objects.push_back(std::move(object));

	return pObject;
}

std::vector<Mage::GameObject*> Mage::Scene::GetObjects() const
{
	std::vector<GameObject*> vecRaw;

	vecRaw.reserve(m_Objects.size());
	std::transform(m_Objects.cbegin(), m_Objects.cend(), std::back_inserter(vecRaw),
		[](auto& ptr) { return ptr.get(); });

	return vecRaw;
}

void Mage::Scene::DrawImGui() const
{
	for (const auto& object : m_Objects)
	{
		object->DrawImGui();
	}
}

void Mage::Scene::Update() const
{
	for (const auto& object : m_Objects)
	{
		object->Update();
	}
}

void Mage::Scene::FixedUpdate() const
{
	for (const auto& object : m_Objects)
	{
		object->FixedUpdate();
	}
}

void Mage::Scene::DestroyMarkedObjects()
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

void Mage::Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}