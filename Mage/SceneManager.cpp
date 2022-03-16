#include "MagePCH.h"
#include "SceneManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "imgui.h"

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

void SceneManager::DrawImGui() const
{
    m_Scenes[m_ActiveScene]->DrawImGui();

	DisplaySceneGraph();
}

void SceneManager::Update() const
{
	m_Scenes[m_ActiveScene]->Update();
}

void SceneManager::FixedUpdate() const
{
	m_Scenes[m_ActiveScene]->FixedUpdate();
}

void SceneManager::DestroyMarkedObjects() const
{
	m_Scenes[m_ActiveScene]->DestroyMarkedObjects();
}

void SceneManager::Render() const
{
	m_Scenes[m_ActiveScene]->Render();
}

Scene* SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::unique_ptr<Scene>(new Scene(name));
	const auto pScene = scene.get();

	m_Scenes.push_back(std::move(scene));

	return pScene;
}

void SceneManager::SetActiveScene(int sceneIndex)
{
	m_ActiveScene = sceneIndex;
}

void SceneManager::DisplaySceneGraph() const
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

inline void SceneManager::DisplayChildObjects(const GameObject* parent) const
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
