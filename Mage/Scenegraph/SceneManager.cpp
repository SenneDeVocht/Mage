#include "Mage/MagePCH.h"
#include "SceneManager.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Scenegraph/Scene.h"

#include "imgui.h"

Mage::SceneManager::SceneManager() = default;
Mage::SceneManager::~SceneManager() = default;

void Mage::SceneManager::DrawImGui() const
{
	m_Scenes[m_ActiveScene]->DrawImGui();
}

void Mage::SceneManager::Update() const
{
	m_Scenes[m_ActiveScene]->Update();
}

void Mage::SceneManager::FixedUpdate() const
{
	m_Scenes[m_ActiveScene]->FixedUpdate();
}

void Mage::SceneManager::Render() const
{
	m_Scenes[m_ActiveScene]->Render();
}

void Mage::SceneManager::RenderGizmos() const
{
	m_Scenes[m_ActiveScene]->RenderGizmos();
}

void Mage::SceneManager::ChangeSceneGraph() const
{
	m_Scenes[m_ActiveScene]->ChangeSceneGraph();
}

Mage::Scene* Mage::SceneManager::CreateScene(const std::string & name)
{
	auto scene = std::unique_ptr<Scene>(new Scene(name));
	const auto pScene = scene.get();

	m_Scenes.push_back(std::move(scene));

	return pScene;
}

void Mage::SceneManager::SetActiveScene(int sceneIndex)
{
	m_ActiveScene = sceneIndex;
}