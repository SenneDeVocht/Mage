#include "MagePCH.h"
#include "SceneManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "imgui.h"

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

void SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}

	DisplaySceneGraph();
}

void SceneManager::FixedUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void SceneManager::DestroyMarkedObjects()
{
	for (const auto& scene : m_Scenes)
	{
		scene->DestroyMarkedObjects();
	}
}

void SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

Scene* SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::unique_ptr<Scene>(new Scene(name));
	const auto pScene = scene.get();

	m_Scenes.push_back(std::move(scene));

	return pScene;
}

void SceneManager::DisplaySceneGraph() const
{
	ImGui::Begin("Scene Graph");

	// Scenes
	for (const auto& scene : m_Scenes)
	{
		if (ImGui::TreeNode("Scene"))
		{
			// Root Objects
			auto objects = scene->GetObjects();
			for (const auto& rootObject : objects)
			{
				if (ImGui::TreeNode(rootObject->GetName().c_str()))
				{
					// Display all children recursively
					DisplayChildObjects(rootObject);
					ImGui::TreePop();
				}
			}

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
