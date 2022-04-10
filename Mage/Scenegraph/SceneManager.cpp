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

	DisplaySceneGraph();
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

void Mage::SceneManager::DisplaySceneGraph() const
{
	ImGui::Begin(m_Scenes[m_ActiveScene]->GetName().c_str());

	// Root Objects
	auto objects = m_Scenes[m_ActiveScene]->GetObjects();
	for (const auto& rootObject : objects)
	{
		if (ImGui::TreeNode(rootObject->GetName().c_str()))
		{
			// Display all children recursively
			DisplayChildObjects(rootObject);
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

inline void Mage::SceneManager::DisplayChildObjects(const GameObject * parent) const
{
	const auto children = parent->GetChildren();
	for (const auto& child : children)
	{
		// Display self, then show children recursively
		if (ImGui::TreeNode(child->GetName().c_str()))
		{
			DisplayChildObjects(child);
			ImGui::TreePop();
		}
	}
}